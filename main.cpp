#include <iostream>
#include <Eigen/Eigen>
#include "vp/types.h"
#include "io/numpy/array_io.h"
#include "vp/utils/utils.h"
#include "vp/fundamental_matrix_estimator.h"


int main(int, char**) {
    std::cout << "Loading input data!\n";
    
    std::string dataX1FilePath = "/home/diego/dev/VisualPerception/main/data_x1.npy";
    std::string dataX2FilePath = "/home/diego/dev/VisualPerception/main/data_x2.npy";
    // std::string dataX3FilePath = "/home/diego/dev/VisualPerception/data_x3.npy";

    io::numpy::Array<double, 2> x2;
    io::numpy::ArrayIO::deserialize(dataX2FilePath, x2);
    Eigen::MatrixXd x2Matrix = vp::numpyArrayToMatrix<double, 2>(x2);

    io::numpy::Array<double, 2> x1;
    io::numpy::ArrayIO::deserialize(dataX1FilePath, x1);
    Eigen::MatrixXd x1Matrix = vp::numpyArrayToMatrix<double, 2>(x1);

    std::cout << "Input data read!!\n";

    std::cout << "Estimating Fundamental matrix!\n";    
    vp::FundamentalMatrixEstimator estimator;
    Eigen::Matrix3d F = estimator.estimate(x1Matrix, x2Matrix);
    std::cout << F << "\n";
}
