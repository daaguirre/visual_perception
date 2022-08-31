#include "fundamental_matrix_estimator.h"

#include <Eigen/Eigen>
#include <Eigen/SVD>

namespace vp
{

FundamentalMatrixEstimator::FundamentalMatrixEstimator() {}

Matrix33 FundamentalMatrixEstimator::estimate(const Matrix3X& x1, const Matrix3X& x2) const
{
    std::unique_ptr<Eigen::MatrixXd> A_ptr = build_matrix_A(x1, x2);
    Eigen::BDCSVD<Eigen::MatrixXd> svd1(*A_ptr, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Eigen::MatrixXd& V = svd1.matrixV();
    auto full_rank_F =
        Eigen::Map<const Eigen::Matrix<double, 3, 3, Eigen::RowMajor>>(V.col(8).data(), 3, 3);

    // apply SVD again to make sure final matrix will be of rank 2
    Eigen::BDCSVD<Eigen::MatrixXd> svd2(full_rank_F, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Matrix33 diagonal_matrix(svd2.singularValues().asDiagonal());
    diagonal_matrix(2, 2) = 0;
    Matrix33 F = svd2.matrixU() * diagonal_matrix * svd2.matrixV().transpose();
    F.normalize();
    return F;
}

std::unique_ptr<MatrixX> FundamentalMatrixEstimator::build_matrix_A(
    const Matrix3X& x1,
    const Matrix3X& x2) const
{
    std::unique_ptr<Eigen::MatrixXd> A_ptr = std::make_unique<Eigen::MatrixXd>(x1.cols(), 9);
    A_ptr->setZero();
    A_ptr->col(0) = x1.row(0).cwiseProduct(x2.row(0));
    A_ptr->col(1) = x1.row(0).cwiseProduct(x2.row(1));
    A_ptr->col(2) = x1.row(0);
    A_ptr->col(3) = x1.row(1).cwiseProduct(x2.row(0));
    A_ptr->col(4) = x1.row(1).cwiseProduct(x2.row(1));
    A_ptr->col(5) = x1.row(1);
    A_ptr->col(6) = x2.row(0);
    A_ptr->col(7) = x2.row(1);
    A_ptr->col(8).setOnes();
    return A_ptr;
}

}  // namespace vp
