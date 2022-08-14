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
    std::unique_ptr<Eigen::MatrixXd> A_ptr = buildMatrixA(x1, x2);
    Eigen::BDCSVD<Eigen::MatrixXd> svd1(*A_ptr, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::MatrixXd& V = svd1.matrixV();
    auto full_rank_F = Eigen::Map<const Eigen::Matrix<double, 3, 3, Eigen::RowMajor>>(V.col(8).data(), 3, 3);
    
    // apply SVD again to make sure final matrix will be of rank 2
    Eigen::BDCSVD<Eigen::MatrixXd> svd2(full_rank_F, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix3d diagonal_matrix(svd2.singularValues().asDiagonal());
    diagonal_matrix(2, 2) = 0;
    Eigen::Matrix3d F = svd2.matrixU() * diagonal_matrix * svd2.matrixV().transpose();
    F.normalize();
    return F;
}

std::unique_ptr<Eigen::MatrixXd> FundamentalMatrixEstimator::buildMatrixA(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const
{
    std::unique_ptr<Eigen::MatrixXd> A_ptr = std::make_unique<Eigen::MatrixXd>(x1.rows(), 9);
    A_ptr->setZero();
    A_ptr->col(0) = x1.col(0).cwiseProduct(x2.col(0));
    A_ptr->col(1) = x1.col(0).cwiseProduct(x2.col(1));
    A_ptr->col(2) = x1.col(0);
    A_ptr->col(3) = x1.col(1).cwiseProduct(x2.col(0));
    A_ptr->col(4) = x1.col(1).cwiseProduct(x2.col(1));
    A_ptr->col(5) = x1.col(1);
    A_ptr->col(6) = x2.col(0);
    A_ptr->col(7) = x2.col(1);
    A_ptr->col(8).setOnes();
    return A_ptr;
}

}
