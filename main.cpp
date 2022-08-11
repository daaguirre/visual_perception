#include <iostream>
#include <Eigen/Eigen>
#include "src/io/numpy/ArraySerializer.h"
// #include "src/FundamentalMatrixEstimator.h"


int main(int, char**) {
    std::cout << "Loading input data!\n";
    
    std::string dataX1FilePath = "/home/diego/dev/RoboticsPerception/data_x1.npy";
    std::string dataX2FilePath = "/home/diego/dev/RoboticsPerception/data_x2.npy";
    std::string dataX3FilePath = "/home/diego/dev/RoboticsPerception/data_x3.npy";
    numpy::Array<double, 2> x1;
    numpy::ArraySerializer::deserialize(dataX1FilePath, x1);
    numpy::Array<double, 2> x2;
    numpy::ArraySerializer::deserialize(dataX2FilePath, x2);
    numpy::Array<double, 2> x3;
    numpy::ArraySerializer::deserialize(dataX3FilePath, x3);

    std::cout << "Input data read!!\n";

    std::cout << "Estimating Fundamental matrix!\n";
    // FundamentalMatrixEstimator estimator();

}
