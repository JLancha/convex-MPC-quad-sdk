import pybullet as pb
import time
import pybullet_data
import numpy as np
import rospy


from quad_msgs.msg import RobotState, MultiFootState, FootState, BodyState
from sensor_msgs.msg import JointState


# ------------------------------------------------------------------------------------

# Written specifically for 12 Dof quadrupedal robots, with potentially extra joints
# Could replace contact_state_publisher? Publish directly into state/grfs?



# Topics to use
#  * /robot_1/state/ground_truth [quad_msgs/RobotState]
#  * /robot_1/state/ground_truth_body_frame [quad_msgs/RobotState]


# -----------------------------------------------------------------------------

# Some utility functions using pybullet

def pos_quat_to_SE3(pos,quat):
    SO3 = (np.array(pb.getMatrixFromQuaternion(quat))).reshape([3,3]) 
    P = np.array(pos).reshape([3,1])
    SE3 = np.r_[np.c_[SO3,P],np.array([[0,0,0,1]])] # Join cols and rows together
    return SE3

def SE3_transform_pos(pos_vec,pos,quat):
    homo_pos_vec = np.append(np.array(pos_vec),1).reshape([4,1]) # turn vector into homogeneous coordinate form
    SO3 = (np.array(pb.getMatrixFromQuaternion(quat))).reshape([3,3])
    P = np.array(pos).reshape([3,1])
    SE3 = np.r_[np.c_[SO3,P],np.array([[0,0,0,1]])] # construct SE3 matrix
    transformed_vec = SE3@homo_pos_vec
    return transformed_vec[:3] # Return 3x1, same dimension as 3x1 input pos_vec


#--------------------------------------------------------------------------------
# Only need to publish to /robot_1/state/ground_truth [quad_msgs/RobotState]?


class Robot_sensors:

    def __init__(self,robot,node = None,joint_topic = None,ground_truth_topics = None,contact_topics= None):


        # self.jointPub = rospy.Publisher(joint_topic,JointState)
        # self.ground_truth_Pub = rospy.Publisher(ground_truth_topics[0],RobotState) 
        # self.ground_truth_body_Pub = rospy.Publisher(ground_truth_topics[1],RobotState) 
        # self.contactPubs = [None for j in range(len(contact_topics))]
        # for i in range(len(contact_topics)):

        self.robot = robot # Get bodyID from higher level node


        nJoints =pb.getNumJoints(self.robot)
        jointNameToId ={}
        for i in range (nJoints):
            jointInfo =pb.getJointInfo(self.robot, i)
            jointNameToId[jointInfo[1].decode('UTF-8')] = jointInfo[0] 
        # spine=jointNameToId["spine"]

        # Leg0 = FL
        abdfl=jointNameToId["8"]
        hipfl=jointNameToId["0"]
        kneefl=jointNameToId["1"]

        # Leg1 = BL
        abdbl=jointNameToId["9"]
        hipbl=jointNameToId["2"]
        kneebl=jointNameToId["3"]

        # Leg2 = FR
        abdfr=jointNameToId["10"]
        hipfr=jointNameToId["4"]
        kneefr=jointNameToId["5"]

        # Leg3 = BR
        abdbr=jointNameToId["11"]
        hipbr=jointNameToId["6"]
        kneebr=jointNameToId["7"]

        legFR = [abdfr,hipfr,kneefr]
        legFL = [abdfl,hipfl,kneefl]
        legBR = [abdbr,hipbr,kneebr]
        legBL = [abdbl,hipbl,kneebl]

        jtoes = [jointNameToId['jtoe%d'%i] for i in range(4)]
        print(jtoes)
        self.toeIdx = jtoes
        self.leg_joint_ids = [legFL,legBL,legFR,legBR]
        self.basic_joint_ids = legFL+legBL+legFR+legBR
        self.basic_joint_names = \
            ["8","0","1","9","2","3",\
            "10","4","5","11","6","7"]

        self.all_joint_names = list(jointNameToId.keys())
        self._nameToId = jointNameToId

    def get_basic_joints2(self):
        names = self.basic_joint_names
        jointStates =pb.getJointStates(self.robot,self.basic_joint_ids)
        # print(len(jointStates[0]))
        positions, vels, jointRFs, effort_pb = jointStates
        efforts = [0.0*len(self.basic_joint_ids)]  # Hardcode as 0.0, same as in gazebo estimator_plugin.cpp
        # effort_pb = effort measured in Pybullet
        return [names,positions,vels,efforts]
        # return [names,positions,vels,efforts,jointStates]
    
    def get_basic_joints(self):
        positions = []
        vels = []
        efforts = [0.0*len(self.basic_joint_ids)]
        for i in self.basic_joint_ids:
            pos_i,vel_i,rf_i,eff_i =pb.getJointState(self.robot,i)
            positions.append(pos_i)
            vels.append(vel_i)
        return [self.basic_joint_names,positions,vels,efforts]


    def get_single_toe(self,idx):
        toe = self.toeIdx[idx]
        toe_state =pb.getLinkState(self.robot,toe, computeLinkVelocity = 1)
        location = toe_state[0] # Only need world frame pos
        vel = toe_state[6] # world frame linear vel
        contacting = self.get_single_contact_state(idx) # bool
        acc = (0.0,0.0,0.0) # hardcode 0

        return [location,vel,acc,contacting]

    def get_single_contact_state(self,idx):
        toe = self.toeIdx[idx]
        contact =pb.getContactPoints(bodyB = self.robot,linkIndexB = toe)
        if len(contact) >0:
            contact_state = True
        else:
            contact_state = False
        return contact_state


    def get_all_toes(self):
        toe_states = []
        for toe in self.toeIdx:
            toestate = self.get_single_toe(toe)
            toe_states.append(toestate)
        return toe_states

    def get_body_state(self):
        body_state =pb.getLinkState(self.robot,0, computeLinkVelocity = 1)
        location = body_state[0] # Only need world frame pos
        orientation = body_state[1]
        linear_vel = body_state[6] # world frame linear vel
        angular_vel = body_state[7] # world frame ang vel, cartesian, not twist!
        return [location,orientation,linear_vel,angular_vel]
        # return [location,orientation,linear_vel,angular_vel,body_state]

    def write_RobotState_msg(self):
        
        RobotStatemsg_out= RobotState()
        body_msg = BodyState()
        toe_msgs = MultiFootState()
        joint_msg = JointState()

# BodyState message
        body_state = self.get_body_state()

        body_msg.pose.position.x = body_state[0][0]
        body_msg.pose.position.y = body_state[0][1]
        body_msg.pose.position.z = body_state[0][2]

        body_msg.pose.orientation.x = body_state[1][0]
        body_msg.pose.orientation.y = body_state[1][1]
        body_msg.pose.orientation.z = body_state[1][2]
        body_msg.pose.orientation.w = body_state[1][3]

        body_msg.twist.linear.x = body_state[2][0]
        body_msg.twist.linear.y = body_state[2][1]
        body_msg.twist.linear.z = body_state[2][2]
        
        body_msg.twist.angular.x = body_state[3][0]
        body_msg.twist.angular.y = body_state[3][1]
        body_msg.twist.angular.z = body_state[3][2]

# JointState message
        joint_states = self.get_basic_joints()
        joint_msg.name = joint_states[0]
        joint_msg.position = joint_states[1]
        joint_msg.velocity = joint_states[2]
        joint_msg.effort = joint_states[3]

# MultiFootState messages
        # toe_states = self.get_all_toes()
        all_feet_msgs = []
        # dummy = (0,0,0)
        for i in range(3):
            foot_i = FootState()
            toe_state = self.get_single_toe(i)
            foot_i.position.x,foot_i.position.y,foot_i.position.z = toe_state[0]
            foot_i.velocity.x,foot_i.velocity.y,foot_i.velocity.z = toe_state[1]
            foot_i.acceleration.x,foot_i.acceleration.y,foot_i.acceleration.z = toe_state[2]
            foot_i.contact = toe_state[3]

            all_feet_msgs.append(foot_i)
        toe_msgs.feet = all_feet_msgs

# Join msgs together
        RobotStatemsg_out.body = body_msg
        RobotStatemsg_out.joints = joint_msg
        RobotStatemsg_out.feet = toe_msgs
        return  RobotStatemsg_out


