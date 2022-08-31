#ifndef __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_
#define __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_

#include <Eigen/Eigen>
#include <cstring>
#include <utility>

#include "types.h"

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
     * @return Matrix33 3x3 essential matrix with singular values (1, 1, 0) rank 2
     */
    Matrix33 estimate_from_fundamental_matrix(const Matrix33& f, const Matrix33& k) const;
};

}  // namespace vp

#endif  // __FIXTURES_VP_ESSENTIAL_MATRIX_ESTIMATOR_H_