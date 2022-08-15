#include "essential_matrix_estimator.h"

namespace vp
{

Eigen::Matrix3d EssentialMatrixEstimator::estimateFromFundamentalMatrix(
    const Eigen::Matrix3d& F, const Eigen::Matrix3d& K) const
{
    Eigen::Matrix3d full_rank_e = K.transpose() * F * K;
    Eigen::BDCSVD<Eigen::MatrixXd> svd(full_rank_e, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Matrix3d diagonal_matrix(svd.singularValues().asDiagonal());
    diagonal_matrix(2, 2) = 0;
    Eigen::Matrix3d E = svd.matrixU() * diagonal_matrix * svd.matrixV().transpose();
    E.normalize();
    return E;
}

}  // namespace vp