#ifndef __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_
#define __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_

#include <Eigen/Eigen>
#include <cstring>
#include <utility>

namespace vp
{

class EssentialMatrixEstimator
{
public:

    /**
     * @brief estimates the essential matrix E from the fundamental matrix (F) as follows
     * E = K'FK
     * 
     * @param f fundamental matrix 3x3 
     * @param k camera intrinsic parameter cmatrix 3x3
     * @return Eigen::Matrix3d 3x3 essential matrix with singular values (1, 1, 0) rank 2
     */
    Eigen::Matrix3d estimateFromFundamentalMatrix(const Eigen::Matrix3d& f,
                                                  const Eigen::Matrix3d& k) const;
};

}  // namespace vp

#endif  // __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_