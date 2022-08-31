#include "essential_matrix_estimator.h"

namespace vp
{

Matrix33 EssentialMatrixEstimator::estimate_from_fundamental_matrix(
    const Matrix33& F,
    const Matrix33& K) const
{
    Matrix33 full_rank_e = K.transpose() * F * K;
    Eigen::BDCSVD<Eigen::MatrixXd> svd(full_rank_e, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Matrix33 diagonal_matrix(svd.singularValues().asDiagonal());
    diagonal_matrix(2, 2) = 0;
    Matrix33 E = svd.matrixU() * diagonal_matrix * svd.matrixV().transpose();
    E.normalize();
    return E;
}

}  // namespace vp