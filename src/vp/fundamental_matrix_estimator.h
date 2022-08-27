#ifndef __FUNDAMENTAL_MATRIX_ESTIMATOR_H_
#define __FUNDAMENTAL_MATRIX_ESTIMATOR_H_

#include <Eigen/Eigen>

#include "types.h"

namespace vp
{

class FundamentalMatrixEstimator
{
public:
    FundamentalMatrixEstimator();

    Matrix33 estimate(const Matrix3X& x1, const Matrix3X& x2) const;

private:
    std::unique_ptr<MatrixX> build_matrix_A(const Matrix3X& x1, const Matrix3X& x2) const;
};

}  // namespace vp

#endif  // __FUNDAMENTAL_MATRIX_ESTIMATOR_H_
