#include "lmpc_controller.h"


int main(int argc, char** argv){
    
    auto start = std::chrono::high_resolution_clock::now();

    MPC mpc;
    mpc.initMatricesZero();

    Eigen::VectorXd q_weights = Eigen::VectorXd::Ones(NUM_STATE);
    mpc.setQMatrix(q_weights);

    Eigen::VectorXd r_weights = Eigen::VectorXd::Ones(NUM_DOF);
    mpc.setRMatrix(r_weights);

    auto Rotation_z = mpc.setRotationMatrix(Eigen::Vector3d(0.5, 0.7, 0.6));
    mpc.setAMatrixContinuous(Rotation_z);
    mpc.setAMatrixDiscrete();
    mpc.setBMatrixContinuous();
    mpc.setBMatrixDiscrete();
    mpc.setAqpMatrix();
    mpc.setBqpMatrix();
    mpc.setAcMatrix();
    mpc.setBounds();
    mpc.setHessian();
    mpc.setGradient();
    mpc.setInitialGuess();

    mpc.solveQP();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Total Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}