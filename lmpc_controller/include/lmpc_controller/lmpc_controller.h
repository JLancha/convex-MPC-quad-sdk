#ifndef LMPC_CONTROLLER_H
#define LMPC_CONTROLLER_H

#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <cmath>
#include <iostream>
#include <chrono>

#include <ros/ros.h>
#include <quad_utils/ros_utils.h>
#include <quad_msgs/RobotState.h>

//Constants - Declared here because inside the class does not work

//g = acceleration of gravity [m/s^2]
const double g = -9.81;

/**
 * NUM_STATE = dimension of state vector (X, Y, Z, Vx, Vy, Vz, θx, θy, θz wx, wy, wz, g)
 * (position, lin. vel., orientation, ang. velocity, gravity)
 * due to the extension to hold the gravity term it comes to dimension 13
 * 
 * foot_positions = 3x4 matrix to hold the xyz coords of each foot. (in the body frame?)
 * 
 * NUM_DOF = Each foot has 1 GRF (which is a 3x1 vector), therefore NUM_DOF = 3*LEGS = 12
*/
const int LEGS = 4;
const int NUM_STATE = 13;
const int NUM_DOF = 3 * LEGS;
/**
 * HORIZON_LENGTH = number of steps in the horizon
 * ROBOT_MASS = Mass of the robot (10 [kg])
*/
const int HORIZON_LENGTH = 10;
const double dt = 0.03;

/**
 * NUM_BOUNDS = Number of bounds for the constraints. In our case 5 since we had to divide each inequality
 *                                                    into 2 bounds (4) + the contact constraint.
 *                                                   (Decide if there is a way to generalize this or user inputs a constant)
*/
const int NUM_BOUNDS = 5;

//A1_INERTIA_BODY = Inertia Matrix of the robot in the body frame [kg*m/s^2].
const Eigen::Matrix3d A1_INERTIA_BODY = (Eigen::Matrix3d() << 
                                           0.0158533, -3.66 * std::pow(10, -5), -6.11 * std::pow(10, -5), 
                                           -3.66 * std::pow(10, -5), 0.0377999, -2.75 * std::pow(10, -5),
                                           -6.11 * std::pow(10, -5), -2.75 * std::pow(10, -5), 0.0456542).finished();

//ROBOT_MASS = Mass of the robot (13.6 [kg])
const double ROBOT_MASS = 13.6;


/**
 * @brief This function converts a 3D vector into a skew-symmetric matrix
 * 
 * @param[in] = vector - 3D vector to be converted.
 * @param[in] = skew_symmetric - The skew symmetric matrix to be populated by the vector components
 * 
 * @param[out] = skew_symmetric - The updated skew symmetric matrix.
 * 
 * @returns = None
*/
void vectorToSkewSymmetric(Eigen::Vector3d vector, Eigen::Matrix3d &skew_symmetric){
    skew_symmetric << 0, -vector(2), vector(1),
                      vector(2), 0, -vector(0),
                      -vector(1), vector(0), 0;
}

/// @brief 
class MPC{
public:
    
    // Constructor
    MPC(ros::NodeHandle &nh, int type){
        
        //ROS Stuff
        nh_ = nh;
        robot_id_ = type;
        // Parameters initialization with values from paper
        mu = 0.3;
        fz_min = 10;
        fz_max = 666;        
    }
    /**
     * Initializes the A_matrix_continuous and A_matrix_discrete matrices to zero.
     * 
     * This function sets the A_matrix_continuous and A_matrix_discrete matrices to zero.
     * A_matrix_continuous is an Eigen::Matrix<double, NUM_STATE, NUM_STATE> matrix,
     * and A_matrix_discrete is an Eigen::Matrix3d matrix.
     */
    void initMatricesZero(){
        A_matrix_continuous = Eigen::Matrix<double, NUM_STATE, NUM_STATE>::Zero();
        A_matrix_discrete = Eigen::Matrix<double, NUM_STATE, NUM_STATE>::Zero();
        B_matrix_continuous = Eigen::Matrix<double, NUM_STATE, NUM_DOF>::Zero();
        B_matrix_continuous_list = Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF>::Zero();
        B_matrix_discrete_list = Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF>::Zero();
        B_matrix_discrete = Eigen::Matrix<double, NUM_STATE, NUM_DOF>::Zero();
        Aqp_matrix = Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_STATE>::Zero();
        Bqp_matrix = Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF * HORIZON_LENGTH>::Zero();
    }
    
    /**
     * @brief This function calculates the average yaw angle (psi) in the whole horizon.
     * 
     * @param ref_body_plan The reference states of the body (each row is a state vector at a given horizon step)
     * @return double The average yaw angle in the whole horizon (psi)
     */
    double extractPsi(Eigen::MatrixXd ref_body_plan){
        double yaw_sum = 0;
        for (int i = 0; i < HORIZON_LENGTH; i++)
        {
            yaw_sum += ref_body_plan(i, 5);
        }
        double psi = yaw_sum / HORIZON_LENGTH;
        return psi;
    }
    
    // A function that change the order of states like that 
    // Quad = [x y z theta_x theta_y theta_z vx vy vz wx wy wz g]
    // A1 = [theta_x theta_y theta_z x y z wx wy wz vx vy vz g]
    void quadToA1ChangeStatesOrder(Eigen::VectorXd &current_state_, Eigen::MatrixXd &states_reference_)
    { 
        quadToA1ChangeCurrentState(current_state_);
        quadToA1ChangeRefState(states_reference_);
    }

    void quadToA1ChangeCurrentState(Eigen::VectorXd &current_state_)
    {
        Eigen::VectorXd temp_current_state = current_state_;
        current_state_.conservativeResize(NUM_STATE);
        current_state_.segment(0, 3) = temp_current_state.segment(3, 3);  // orientation goes to place 1
        current_state_.segment(3, 3) = temp_current_state.segment(0, 3);  // position goes to place 2
        current_state_.segment(6, 3) = temp_current_state.segment(9, 3);  // angular velocity goes to place 3
        current_state_.segment(9, 3) = temp_current_state.segment(6, 3);  // linear velocity goes to place 4
        current_state_(12) = g;
    }

    void quadToA1ChangeRefState(Eigen::MatrixXd &states_reference_)
    {
        Eigen::MatrixXd temp_states_reference = states_reference_;
        // states_reference_.conservativeResize(HORIZON_LENGTH, NUM_STATE);
        states_reference_.conservativeResize(HORIZON_LENGTH, NUM_STATE);


        for (int i=0; i<HORIZON_LENGTH; i++){
            states_reference_.row(i).segment(0, 3) = temp_states_reference.row(i).segment(3, 3);  // orientation goes to place 1
            states_reference_.row(i).segment(3, 3) = temp_states_reference.row(i).segment(0, 3);  // position goes to place 2
            states_reference_.row(i).segment(6, 3) = temp_states_reference.row(i).segment(9, 3);  // angular velocity goes to place 3
            states_reference_.row(i).segment(9, 3) = temp_states_reference.row(i).segment(6, 3);  // linear velocity goes to place 4
            states_reference_.row(i)(12) = g;
            // std::cout << states_reference_.row(i) << std::endl;
        }      
        // std::cout << std::endl;
    }

    void A1ToQuadChangeCurrentState(Eigen::VectorXd &current_state_)
    {
        Eigen::VectorXd temp_current_state = current_state_;
        current_state_.conservativeResize(NUM_STATE-1);
        current_state_.segment(0, 3) = temp_current_state.segment(3, 3);  // position goes to place 1
        current_state_.segment(3, 3) = temp_current_state.segment(0, 3);  // orientation goes to place 2
        current_state_.segment(6, 3) = temp_current_state.segment(9, 3);  // linear velocity goes to place 3
        current_state_.segment(9, 3) = temp_current_state.segment(6, 3);  // angular velocity goes to place 4
    }

    void A1ToQuadChangeRefState(Eigen::MatrixXd &states_reference_)
    {
        Eigen::MatrixXd temp_states_reference = states_reference_;
        states_reference_.conservativeResize(HORIZON_LENGTH, NUM_STATE-1);

        for (int i=0; i<HORIZON_LENGTH; i++){
            states_reference_.row(i).segment(0, 3) = temp_states_reference.row(i).segment(3, 3);  // position goes to place 1
            states_reference_.row(i).segment(3, 3) = temp_states_reference.row(i).segment(0, 3);  // orientation goes to place 2
            states_reference_.row(i).segment(6, 3) = temp_states_reference.row(i).segment(9, 3);  // linear velocity goes to place 3
            states_reference_.row(i).segment(9, 3) = temp_states_reference.row(i).segment(6, 3);  // angular velocity goes to place 4
        }       
    }

    /**
     * @brief Sets the rotation matrix (from body to world frame) based on the given Euler angles.
     * The robot’s orientation is expressed as a vector of Z-Y-X Euler angles Θ = [φ θ ψ]ᵀ where ψ is the yaw, θ is the pitch, and φ is the roll.
     * 
     * @param euler_angles The Euler angles representing the rotation of the body respect to the inertial/world frame.
     */
    Eigen::Matrix3d setRotationMatrix(double average_yaw){
        double psi = average_yaw;
        // std::cout<<"Average yaw: "<<psi<<std::endl;
        // Rotation matrix around the z-axis
        Rotation_z << cos(psi), sin(psi), 0,
                     -sin(psi), cos(psi), 0,
                             0,        0, 1;

        return Rotation_z;
    }

    /**
     * @brief This function initializes a diagonal matrix for weights on the state error
     * It populates Q_matrix with q_weights in its diagonal entries.
     */
    void setQMatrix(){

        // It doesnt let us declare size at the start so we need to initialize it here
        Q_matrix = Eigen::SparseMatrix<double>(NUM_STATE * HORIZON_LENGTH, NUM_STATE * HORIZON_LENGTH);
        for (int i = 0; i < NUM_STATE * HORIZON_LENGTH; i++)
        {
            q_weights << 20.0, 10.0, 1.0, 0.0, 0.0, 420.0, 0.05, 0.05, 0.05, 30.0, 30.0, 10.0, 0;

            // We insert weights in the diagonal of the matrix but we multiply by 2 
            // because the QP solver expects a 0.5 factor in front of the quadratic term
            Q_matrix.insert(i, i) = 2 * q_weights(i % NUM_STATE);
        }
        // std::cout << "Q_matrix: \n" << Q_matrix << std::endl;
        // std::cout << "Q_matrix Shape: \n" << Q_matrix.rows() << " x " << Q_matrix.cols() << std::endl;
    }

    /**
     * @brief This function initializes a diagonal matrix for weights on the control inputs
     * It populates R_matrix with r_weights in its diagonal entries.
     * 
     * @param[in] = r_weights. A vector of weights to be populated in the diagonal of R_matrix
     * @param[out] = R_matrix. The R_matrix for the cost function (diagonal)
     * 
     * @returns = None
    */    
    void setRMatrix(){
        R_matrix = Eigen::SparseMatrix<double>(NUM_DOF * HORIZON_LENGTH, NUM_DOF * HORIZON_LENGTH);
        for (int i = 0; i < NUM_DOF * HORIZON_LENGTH; i++)
        {
            R_matrix.insert(i, i) = 2 * r_weights(i % NUM_DOF);
        }
        // std::cout << "R_matrix: \n" << R_matrix << std::endl;
        // std::cout << "R_matrix Shape: \n" << R_matrix.rows() << " x " << R_matrix.cols() << std::endl;
    }

    /**
     * @brief This function initializes the A matrix of the state space in the continuous time domain
     * The matrix is detailed in the repport
     * 
     * @param[in] = Rotation_z - The rotation matrix from body to world frame, around the Z axis.
     * @param[out] = The A matrix in continuous time domain for the state space.
     * 
     * @returns = None
    */
    void setAMatrixContinuous(Eigen::Matrix3d Rotation_z){
        // Using the paper A matrix as reference
        A_matrix_continuous.block<3, 3>(0, 6) = Rotation_z;
        A_matrix_continuous.block<3, 3>(3, 9) = Eigen::Matrix3d::Identity();
        A_matrix_continuous(11, 12) = 1; // Because of the augmented gravity term in the state space model
        // std::cout << "A_matrix_continuous: \n" << A_matrix_continuous << std::endl;
        // std::cout << "A_matrix_continuous Shape: \n" << A_matrix_continuous.rows() << " x " << A_matrix_continuous.cols() << std::endl;

    }

    /**
     * @brief This function initializes the A matrix of the state space in the discrete time domain
     * The discretization is done by first order approximation of the matrix exponential: 
     * A_discrete = I + A*dt
     * 
     * @param[in] = A_matrix_continuous (The matrix in continuous time domain)
     * @param[in] = dt (the timestep)
     * 
     * @param[out] = A_matrix_discrete (The discretized A_matrix of the state space representation)
     * 
     * @returns = None
    */
    void setAMatrixDiscrete(double &first_element_duration){
        Eigen::MatrixXd A_matrix_continuous_sampled = A_matrix_continuous*dt;
        A_matrix_continuous_sampled.row(0) = A_matrix_continuous.row(0)*first_element_duration;
        A_matrix_discrete = Eigen::Matrix<double, NUM_STATE, NUM_STATE>::Identity(NUM_STATE, NUM_STATE) + A_matrix_continuous_sampled;
        // A_matrix_discrete = Eigen::Matrix<double, NUM_STATE, NUM_STATE>::Identity(NUM_STATE, NUM_STATE) + A_matrix_continuous * dt;
        // std::cout << "A_matrix_discrete: \n" << A_matrix_discrete << std::endl;
        // std::cout << "A_matrix_discrete Shape: \n" << A_matrix_discrete.rows() << " x " << A_matrix_discrete.cols() << std::endl;
        
    }

    /**
     * TODO: Finish documenting this function with parameter description
     * @brief This function sets the B matrix of the state space representation, in the continuous time domain.
     * The matrix is detailed in the repport.
     * 
     * @param[in] = foot_positions - 
     * @param[in] = A1_INERTIA_WORLD - Inertia matrix of the robot in the world frame
     * @param[in] = ROBOT_MASS - Scalar holding the mass of the robot
     * 
     * @param[out] = B_matrix_continuous - the B matrix in the continuous time domain
     * 
     * @returns = None
    */
    void setBMatrixContinuous(Eigen::MatrixXd foot_positions, Eigen::Matrix3d Rotation_z){
       
        Eigen::Matrix3d A1_INERTIA_WORLD;
        A1_INERTIA_WORLD = Rotation_z * A1_INERTIA_BODY * Rotation_z.transpose();
        
        for (int i = 0; i < HORIZON_LENGTH; i++)
        {                
            for (int j=0; j<LEGS; j++)
            {
                //Extract foot position vector in the given horizon step
                Eigen::Vector3d r_leg = foot_positions.row(i).segment(3*j, 3);
                //Declare and load the skew symmetric matrix of the foot position vector
                Eigen::Matrix3d r_leg_skew;
                vectorToSkewSymmetric(r_leg, r_leg_skew);

                //Load the B matrix with the skew symmetric matrix of foot position multiplied by inertia and 1/mass
                B_matrix_continuous.block<3, 3>(6, 3*j) = A1_INERTIA_WORLD.inverse() * Rotation_z*r_leg_skew;
                B_matrix_continuous.block<3, 3>(9, 3*j) = Eigen::Matrix3d::Identity() * (1/ROBOT_MASS);

            }
            B_matrix_continuous_list.block<NUM_STATE, NUM_DOF>(i*NUM_STATE, 0) = B_matrix_continuous;
            
        }
        // std::cout << "B_matrix_continuous: \n" << B_matrix_continuous << std::endl;
        // std::cout << "B_matrix_continuous Shape: \n" << B_matrix_continuous.rows() << " x " << B_matrix_continuous.cols() << std::endl;

        // std::cout << "B_matrix_continuous_list: \n" << B_matrix_continuous_list << std::endl;
        // std::cout << "B_matrix_continuous_list Shape: \n" << B_matrix_continuous_list.rows() << " x " << B_matrix_continuous_list.cols() << std::endl;

    }

    /**
     * @brief This function sets the B matrix of the state space representation, in the discrete time domain.
     * The discretization is done by scaling the B matrix by the timestep: 
     * B_discrete = B*dt
     * 
     * @param[in] = B_matrix_continuous (The matrix in continuous time domain)
     * @param[in] = dt (the timestep)
     * 
     * @param[out] = B_matrix_discrete (The discretized B_matrix of the state space representation)
     * 
     * @returns = None
    */
    void setBMatrixDiscrete(double &first_element_duration){
        Eigen::Matrix<double, NUM_STATE, NUM_DOF> current_B;
        for (int i = 0; i < HORIZON_LENGTH; i++)
        {
            current_B = B_matrix_continuous_list.block<NUM_STATE, NUM_DOF>(i*NUM_STATE,0);
            B_matrix_discrete = current_B * dt;
            B_matrix_discrete.row(0) = current_B.row(0)*first_element_duration;
            B_matrix_discrete_list.block<NUM_STATE, NUM_DOF>(i*NUM_STATE, 0) = B_matrix_discrete;
        }
        // std::cout << B_matrix_discrete_list << std::endl << std::endl;
        
        
        // std::cout << "B_matrix_discrete: \n" << B_matrix_discrete << std::endl;
        // std::cout << "B_matrix_discrete Shape: \n" << B_matrix_discrete.rows() << " x " << B_matrix_discrete.cols() << std::endl;

        // std::cout << "B_matrix_discrete_list: \n" << B_matrix_discrete_list << std::endl;
        // std::cout << "B_matrix_discrete_list Shape: \n" << B_matrix_discrete_list.rows() << " x " << B_matrix_discrete_list.cols() << std::endl;
    }

    /**
     * @brief This function sets the Aqp matrix for the QP problem.
     * In order to represent the MPC as a QP we need to satisfy the solver's default formulation
     * A_qp is detailed in the repport.
     * 
     * The first block (at position 0,0) is the A_discrete matrix
     * The subsequent blocks are the evolution of the previous blocks
     * A_qp(k+2) = A_discrete(k+1) * A_discrete(k)
     * 
     * @param[in] = A_matrix_discrete - The discrete State space matrix A
     * @param[in] = HORIZON_LENGTH - The length of the horizon
     * 
     * @param[out] = A_qp Matrix for the formulation of the problem as a QP.
     * 
     * @returns = None
    */
    void setAqpMatrix(){
        for (int i = 0; i < HORIZON_LENGTH; i++)
        {
            if (i == 0)
            {  
                Aqp_matrix.block<NUM_STATE, NUM_STATE>(i * NUM_STATE, 0) = A_matrix_discrete;
            }
            else   
            {
                Aqp_matrix.block<NUM_STATE, NUM_STATE>(i * NUM_STATE, 0) = Aqp_matrix.block<NUM_STATE, NUM_STATE>((i - 1) * NUM_STATE, 0) * A_matrix_discrete;
            }
        }
        // std::cout << "Aqp_matrix: \n" << Aqp_matrix << std::endl;
        // std::cout << "Aqp_matrix Shape: " << Aqp_matrix.rows() << " x " << Aqp_matrix.cols() << std::endl;
    }

    /**
     * TODO: OPTIMIZE THIS IMPLEMENTATION - Jumps from 2 to 30ms
     * @brief This function sets the Bqp matrix for the QP problem.
     * In order to represent the MPC as a QP we need to satisfy the solver's default formulation
     * B_qp is detailed in the repport.
     * 
     * The diagonal blocks are the B_discrete matrix
     * The off-diagonal blocks are the multiplication of the Aqp matrix and the B_discrete matrix
     * 
     * @param[in] = B_matrix_discrete - The discrete B matrix of the state space representation
     * @param[in] = Aqp_matrix - The matrix that represents the evolution of the state space matrix
     * 
     * @param[out] = Bqp_matrix - The B matrix for the formulation of the problem as a QP.
     * 
     * @returns = None
    */
    void setBqpMatrix(){
        for (int i = 0; i< HORIZON_LENGTH; i++)
        {
            for (int j=0; j<= i; j++){
                if (i - j== 0)
                {
                    Bqp_matrix.block<NUM_STATE, NUM_DOF>(i * NUM_STATE, j*NUM_DOF) = B_matrix_discrete_list.block<NUM_STATE, NUM_DOF>(j*NUM_STATE, 0);
                }
                else
                {   // I am not sure about this part
                    Bqp_matrix.block<NUM_STATE, NUM_DOF>(i * NUM_STATE, j * NUM_DOF) = 
                    Aqp_matrix.block<NUM_STATE, NUM_STATE>((i -j - 1) * NUM_STATE, 0) * B_matrix_discrete_list.block<NUM_STATE, NUM_DOF>(j*NUM_STATE,0);
                }
            }           
        }
        // std::cout << "Bqp_matrix: \n" << Bqp_matrix << std::endl;
        // std::cout << "Bqp_matrix Shape: " << Bqp_matrix.rows() << " x " << Bqp_matrix.cols() << std::endl;
    }

    /** 
     * OSQP QP formulation
     * minimize 0.5 * x^T * P * x + q^T * x
     * subject to l <= A * x <= u
     * So every constraint equality and inequality need to be converted to this form
    */
    
    /**
     * @brief This Function builds the A matrix in OSQP Notation (Here Ac for constraints)
     * Our MPC problem has 3 Inequality constraints and 1 equality constraint, they are:
     * 
     * 1)-mu*fz <= fx <= mu*fz, which becomes (-inf <= fx-mu*fz <= 0) && (0 <= fx + mu*fz <= inf).
     * 2)-mu*fz <= fy <= mu*fz, which becomes (-inf <= fy-mu*fz <= 0) && (0 <= fy + mu*fz <= inf)
     * Here, 1) and 2) are divided into two bounds to fit OSQP default formulation.
     * 
     * Here the switch is taken care of in the bounds, so our constraint becomes fzmin*switch <= fz <= fzmax*switch
     * (Therefore we encapsulate the equality constraint in an inequality constraint that is being handled by the bounds)
     * Di*ui = 0; 
     * 3)fmin <= fz <= fmax
     * 
     * @param[in] = g_block; - Matrix that will be repeatedly added to Ac to account for the constraints we defined in each leg
     * @param[in] = Ac_Matrix; - Sparse matrix populated with g_block in its diagonal
     * 
     * @param[out] = Updated Ac_matrix
     * 
     * @returns = none
     */
    void setAcMatrix(){
        g_block.resize(NUM_BOUNDS , NUM_DOF/LEGS);
        g_block << 1,0,mu,  // fx + mu*fz
                   1,0,-mu, // fx - mu*fz
                   0,1,mu,  // fy + mu*fz
                   0,1,-mu, // fy - mu*fz
                   0,0,1;   // min max fz/contact constraint (just take into bounds of fz if foot is on ground)

        Ac_matrix = Eigen::SparseMatrix<double>(NUM_BOUNDS * LEGS * HORIZON_LENGTH, NUM_DOF * HORIZON_LENGTH);
        std::vector<Eigen::Triplet<double>> tripletList;
        
        // reserve space for the triplets (40 blocks with 9 non zero entries each)
        tripletList.reserve(9 * 40);  

        int row_offset = 0;
        int col_offset = 0;

        while (row_offset + g_block.rows() <= Ac_matrix.rows() && col_offset + g_block.cols() <= Ac_matrix.cols())
        {
            for (int i = 0; i < g_block.rows(); ++i)
            {
                for (int j = 0; j < g_block.cols(); ++j)
                {
                    tripletList.push_back(Eigen::Triplet<double>(row_offset + i, col_offset + j, g_block(i, j)));
                }
            }
            
            //Move to next insertion points, to begin inserting the next block
            row_offset += 5; 
            col_offset += 3; 
        }

        Ac_matrix.setFromTriplets(tripletList.begin(), tripletList.end());
        Ac_matrix.makeCompressed();
        
        //std::cout << "Ac_matrix: \n" << Ac_matrix << std::endl;
        // std::cout << "Ac_matrix Shape: " << Ac_matrix.rows() << " x " << Ac_matrix.cols() << std::endl;
    }

    /**
     * @brief This Function builds the bounds for the OSQP problem
     * Fitting the OSQP default formulation given by lb <= A_c <= ub
     * 
     * @param[in] = fz_min, fz_max - Actuator limits (indirectly as maximum normal force the robot is capable to generate)
     * @param[in] = mu - Friction coefficient
     * 
     * @param[out] = lower_bounds, upper_bounds - The vectors containing the bounds for the OSQP problem
    */
    void setBounds(std::vector<std::vector<bool>> contact){
        
        // Declaring the lower and upper bounds
        Eigen::VectorXd lower_bounds(NUM_BOUNDS * LEGS);
        Eigen::VectorXd upper_bounds(NUM_BOUNDS * LEGS);
        
        lower_bounds_horizon = Eigen::VectorXd::Zero(NUM_BOUNDS * LEGS * HORIZON_LENGTH);
        upper_bounds_horizon = Eigen::VectorXd::Zero(NUM_BOUNDS * LEGS * HORIZON_LENGTH);

        int horizon_step = 0;
        
        while (horizon_step < HORIZON_LENGTH)
        {
            for (int i=0; i<LEGS; i++)
            {
            lower_bounds.segment(i*NUM_BOUNDS, 5) << 0,                                         //  0        <= fx + mu*fz
                                                    -std::numeric_limits<double>::infinity(),  // -infinity <= fx - mu*fz
                                                     0,                                         //  0        <= fy + mu*fz
                                                    -std::numeric_limits<double>::infinity(),  // -infinity <= fy - mu*fz
                                                    fz_min*contact[horizon_step][i];           //  fz_min   <= fz          fz_min*contact = 0 or 1 depending on the contact
                                                     
            upper_bounds.segment(i*NUM_BOUNDS, 5) << std::numeric_limits<double>::infinity(),   //  fx + mu*fz <= infinity
                                                     0,                                         //  fx - mu*fz <= 0
                                                     std::numeric_limits<double>::infinity(),   //  fy + mu*fz <= infinity
                                                     0,                                         //  fy - mu*fz <= 0
                                                     fz_max*contact[horizon_step][i];           //  fz <= fz_max            fz_max*contact = 0 or 1 depending on the contact
                                                     std::numeric_limits<double>::infinity();           //  fz <= fz_max            fz_max*contact = 0 or 1 depending on the contact
            }

            lower_bounds_horizon.segment(horizon_step*NUM_BOUNDS*LEGS, NUM_BOUNDS * LEGS) = lower_bounds;
            upper_bounds_horizon.segment(horizon_step*NUM_BOUNDS*LEGS, NUM_BOUNDS * LEGS) = upper_bounds;
            horizon_step += 1;
        }
        
        // std::cout<<"Lower bounds: \n"<<lower_bounds_horizon<<std::endl;
        // std::cout<<"Upper bounds: \n"<<upper_bounds_horizon<<std::endl;

        // std::cout << "lower_bounds_horizon Shape: " << lower_bounds_horizon.rows() << " x " << lower_bounds_horizon.cols() << std::endl;
        // std::cout << "upper_bounds_horizon Shape: " << upper_bounds_horizon.rows() << " x " << upper_bounds_horizon.cols() << std::endl;
    }

    /**
     * @brief This function calculates the Hessian of the cost function
     * The mathematical proof for the derivation of this expression can be seen in the repport (and in miro)
     * 
     * @param[in] = Bqp_matrix , Q_matrix, R_matrix
     * @param[out] = hessian of the cost function
     * 
     * @returns = none
    */
    void setHessian(){   
        hessian = Bqp_matrix.transpose() * Q_matrix * Bqp_matrix + R_matrix;
    }

    /**
     * @brief This function calculates the gradient of the cost function
     * The mathematical proof for the derivation of this expression can be seen in the repport (and in miro)
     * It is vital to precompute the hessian (saves around 24ms in the overall implementation)
     * 
     * @param[in] = Bqp_matrix , Q_matrix, hessian, U_vector, states, states_reference
     * @param[out] = gradient of the cost function
     * 
     * @returns = none
    */
    void setGradient(Eigen::MatrixXd grf_plan_, Eigen::VectorXd current_state_, Eigen::MatrixXd ref_body_plan_){
        // Change the order of the states to match the order of our QP formulation
        quadToA1ChangeStatesOrder(current_state_, ref_body_plan_);
        // std::cout << current_state_ << std::endl << std::endl;
        // std::cout << ref_body_plan_ << std::endl << std::endl;
        
        
        gradient.resize(NUM_DOF * HORIZON_LENGTH, 1);
        
        //Manipulate the inputs to satisfy our QP Implementation
        // states = current_state_;
        // states_reference = ref_body_plan_.row(0).transpose();

        Eigen::VectorXd states_ref_stacked = Eigen::VectorXd::Zero(NUM_STATE * HORIZON_LENGTH);
        for (int i = 0; i < HORIZON_LENGTH; i++){
            states_ref_stacked.segment(i*NUM_STATE, NUM_STATE) = ref_body_plan_.row(i).transpose();
        }

        gradient = Bqp_matrix.transpose() * Q_matrix * ((Aqp_matrix *current_state_) - states_ref_stacked);
        // std::cout << gradient << std::endl << std::endl;


        //std::cout << "Gradient: \n" << gradient << std::endl;
        //std::cout << "Shape: " << gradient.rows() << " x " << gradient.cols() << std::endl; 
    }


    /**
     * @brief This function computes the rollout of the MPC controller
     * 
     * @param[in] = u - The vector of control inputs
     * 
     * @param[out] = The rollout of the controller (the predicted states)
     * 
     * @param[in] = current_state - The current state of the robot
     * 
     * @returns = None
    */
    void computeRollout(Eigen::VectorXd &u, Eigen::MatrixXd &x, Eigen::VectorXd current_state)
    {
        x.row(0) = current_state.transpose();

        // Change state order
        quadToA1ChangeCurrentState(current_state);

        Eigen::VectorXd x_next = current_state;
        for (int i = 1; i < HORIZON_LENGTH; i++)
        {
            x_next = A_matrix_discrete * x_next + B_matrix_discrete_list.block<NUM_STATE, NUM_DOF>((i-1)*NUM_STATE, 0) * u.segment((i-1) * NUM_DOF, NUM_DOF);            
            x.row(i) = x_next.transpose();
        }
        // Revert state back to original order
        A1ToQuadChangeRefState(x);
    }

    void computeRollout(Eigen::MatrixXd &u, Eigen::MatrixXd &x, Eigen::VectorXd current_state)
    {
        x.conservativeResize(HORIZON_LENGTH, NUM_STATE);

        // Change state order
        quadToA1ChangeCurrentState(current_state);
        x.row(0) = current_state.transpose();

        Eigen::VectorXd x_next = current_state;
        for (int i = 1; i < HORIZON_LENGTH; i++)
        {          
            x_next = A_matrix_discrete * x_next + B_matrix_discrete_list.block<NUM_STATE, NUM_DOF>((i-1)*NUM_STATE, 0) * u.row(i-1).transpose();
            x.row(i) = x_next.transpose();
        }
        // Revert state back to original order
        A1ToQuadChangeRefState(x);
    }


    Eigen::VectorXd solveQP(){
        //Instantiate the solver
        OsqpEigen::Solver solver;

        auto t0 = std::chrono::high_resolution_clock::now();
        //Configure the solver
        if (!solver.isInitialized()){
            solver.settings()->setVerbosity(false);
            solver.settings()->setWarmStart(true);
            solver.data()->setNumberOfVariables(NUM_DOF*HORIZON_LENGTH);
            solver.data()->setNumberOfConstraints(NUM_BOUNDS*LEGS*HORIZON_LENGTH);
            solver.data()->setLinearConstraintsMatrix(Ac_matrix);
            solver.data()->setHessianMatrix(hessian);
            solver.data()->setGradient(gradient);
            solver.data()->setLowerBound(lower_bounds_horizon);
            solver.data()->setUpperBound(upper_bounds_horizon);
            solver.initSolver();
            }
        else{
            solver.updateGradient(gradient);
            solver.updateHessianMatrix(hessian);
            solver.updateBounds(lower_bounds_horizon, upper_bounds_horizon);
            }

        //Init and solve keeping track of time at each step
    
        auto t1 = std::chrono::high_resolution_clock::now();
        solver.solveProblem();
        auto t2 = std::chrono::high_resolution_clock::now();

        //Process and print time intervals of the solver
        std::chrono::duration<double, std::milli> init_duration = t1 - t0;
        std::chrono::duration<double, std::milli> solve_duration = t2 - t1;
        // std::cout << "Solver init time: " << init_duration.count() << "ms" << std::endl; 
        // std::cout << "Solve time: " << solve_duration.count() << "ms" << std::endl;

        Eigen::VectorXd result = solver.getSolution();

        // // Rotate result grfs to the body frame
        // for (int i = 0; i< 3*LEGS*HORIZON_LENGTH; i+=3){
        //     Eigen::Vector3d grf = result.segment(i, 3); //extract a grf vector
        //     result.segment(i, 3) = Rotation_z.transpose() * grf;    //rotate the grf vector
        // }

        // Print results
        // std::cout << "Result Shape: " << result.rows() << " x " << result.cols() << std::endl;
        // std::cout << "---------------------------------------------------------" << std::endl;
        // std::cout << "Result: " << result.head(12).transpose() << std::endl;
        std::cout << "Rotation Matrix: \n" << Rotation_z << std::endl;  
        return result;
    }

    void printResults()
    {}

    //ROS Stuff
    ros::NodeHandle nh_;
    int robot_id_;

    // Parameters
    double mu;
    double fz_min;
    double fz_max;
    
    //Matrices declaration
    Eigen::Matrix<double, NUM_STATE, NUM_STATE> A_matrix_continuous;
    Eigen::Matrix<double, NUM_STATE, NUM_STATE> A_matrix_discrete;
    Eigen::Matrix<double, NUM_STATE, NUM_DOF> B_matrix_continuous;
    Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF> B_matrix_continuous_list;
    Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF> B_matrix_discrete_list;
    Eigen::Matrix<double, NUM_STATE, NUM_DOF> B_matrix_discrete; 
    Eigen::SparseMatrix<double> Q_matrix;
    Eigen::SparseMatrix<double> R_matrix;
    Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_STATE> Aqp_matrix;
    Eigen::Matrix<double, NUM_STATE * HORIZON_LENGTH, NUM_DOF * HORIZON_LENGTH> Bqp_matrix;

    Eigen::Matrix<double,NUM_BOUNDS , NUM_DOF/LEGS> g_block;
    Eigen::SparseMatrix<double> Ac_matrix;
    
    Eigen::VectorXd lower_bounds_horizon;
    Eigen::VectorXd upper_bounds_horizon;
    
    Eigen::Matrix<double, NUM_DOF * HORIZON_LENGTH, 1> gradient;
    Eigen::SparseMatrix<double> hessian;

    Eigen::Matrix<double, 3, 3> Rotation_z;

    bool is_first_run = true;  //to be set to false after first iteration, so that the initial guess is correctly set to hot-start the solver

    Eigen::VectorXd q_weights = Eigen::VectorXd(NUM_STATE);
    Eigen::VectorXd r_weights = Eigen::VectorXd::Ones(NUM_DOF)*0.00000001;
};

#endif //LMPC_CONTROLLER_H