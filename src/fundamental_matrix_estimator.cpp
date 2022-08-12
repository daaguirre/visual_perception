#include "fundamental_matrix_estimator.h"
#include <Eigen/SVD>
#include <Eigen/Eigen>

namespace vp
{

FundamentalMatrixEstimator::FundamentalMatrixEstimator()
{

}

Eigen::Matrix3d FundamentalMatrixEstimator::estimate(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const
{
    std::unique_ptr<Eigen::MatrixXd> APtr = buildMatrixA(x1, x2);
    Eigen::BDCSVD<Eigen::MatrixXd> svd1(*APtr, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::MatrixXd& V = svd1.matrixV();
    auto fullRankF = Eigen::Map<const Eigen::Matrix<double, 3, 3, Eigen::RowMajor>>(V.col(8).data(), 3, 3);
    
    // apply SVD again to make sure final matrix will be of rank 2
    Eigen::BDCSVD<Eigen::MatrixXd> svd2(fullRankF, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix3d diagonalMatrix(svd2.singularValues().asDiagonal());
    diagonalMatrix(2, 2) = 0;
    Eigen::Matrix3d F = svd2.matrixU() * diagonalMatrix * svd2.matrixV().transpose();
    F.normalize();
    return F;
}

std::unique_ptr<Eigen::MatrixXd> FundamentalMatrixEstimator::buildMatrixA(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const
{
    std::unique_ptr<Eigen::MatrixXd> APtr = std::make_unique<Eigen::MatrixXd>(x1.rows(), 9);
    APtr->setZero();
    APtr->col(0) = x1.col(0).cwiseProduct(x2.col(0));
    APtr->col(1) = x1.col(0).cwiseProduct(x2.col(1));
    APtr->col(2) = x1.col(0);
    APtr->col(3) = x1.col(1).cwiseProduct(x2.col(0));
    APtr->col(4) = x1.col(1).cwiseProduct(x2.col(1));
    APtr->col(5) = x1.col(1);
    APtr->col(6) = x2.col(0);
    APtr->col(7) = x2.col(1);
    APtr->col(8).setOnes();
    return APtr;
}

}
