#include <iostream>
#include <Eigen/Eigen>
#include "src/Types.h"
#include "src/io/numpy/ArraySerializer.h"
#include "src/utils/utils.h"
#include "src/fundamental_matrix_estimator.h"


int main(int, char**) {
    std::cout << "Loading input data!\n";
    
    std::string dataX1FilePath = "/home/diego/dev/VisualPerception/main/data_x1.npy";
    std::string dataX2FilePath = "/home/diego/dev/VisualPerception/main/data_x2.npy";
    // std::string dataX3FilePath = "/home/diego/dev/VisualPerception/data_x3.npy";

    numpy::Array<double, 2> x2;
    numpy::ArraySerializer::deserialize(dataX2FilePath, x2);
    Eigen::MatrixXd x2Matrix = vp::numpyArrayToMatrix<double, 2>(x2, x2.dimension(0), x2.dimension(1));

    numpy::Array<double, 2> x1;
    numpy::ArraySerializer::deserialize(dataX1FilePath, x1);
    Eigen::MatrixXd x1Matrix = vp::numpyArrayToMatrix<double, 2>(x1, x1.dimension(0), x1.dimension(1));

    std::cout << "Input data read!!\n";

    std::cout << "Estimating Fundamental matrix!\n";    
    vp::FundamentalMatrixEstimator estimator;
    Eigen::Matrix3d F = estimator.estimate(x1Matrix, x2Matrix);
    std::cout << F << "\n";
}
