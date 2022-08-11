#include "FundamentalMatrixEstimator.h"
#include <Eigen/SVD>


FundamentalMatrixEstimator::FundamentalMatrixEstimator()
{

}

Ptr<Eigen::Matrix3d> FundamentalMatrixEstimator::estimate(const Eigen::MatrixX2d& x1, const Eigen::MatrixX2d& x2)
{
    Ptr<Eigen::MatrixXd> pA = buildMatrixA(x1, x2);
    Eigen::BDCSVD<Eigen::MatrixXd> svd(*pA, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::MatrixXcd V = svd.matrixV();
    Ptr<Eigen::Matrix3d> pF = std::make_shared<Eigen::Matrix3d>();
    // *pF << V.col(8);
    return pF;
}

Ptr<Eigen::MatrixXd> FundamentalMatrixEstimator::buildMatrixA(const Eigen::MatrixX2d& x1, const Eigen::MatrixX2d& x2)
{
    Ptr<Eigen::MatrixXd> pA = std::make_shared<Eigen::MatrixXd>(x1.rows(), 9);
    pA->setZero();
    pA->col(0) = x1.col(0) * x2.col(0);
    pA->col(1) = x1.col(0) * x2.col(1);
    pA->col(2) = x1.col(0);
    pA->col(3) = x1.col(1) * x2.col(0);
    pA->col(4) = x1.col(1) * x2.col(1);
    pA->col(5) = x1.col(1);
    pA->col(6) = x2.col(0);
    pA->col(7) = x2.col(1);
    pA->col(8).setOnes();
    return pA;
}
