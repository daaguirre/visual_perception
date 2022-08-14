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

    Eigen::Matrix3d estimate(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const;

private:
    std::unique_ptr<Eigen::MatrixXd> buildMatrixA(const Eigen::MatrixXd& x1,
                                                  const Eigen::MatrixXd& x2) const;
};

}  // namespace vp

#endif  // __FUNDAMENTAL_MATRIX_ESTIMATOR_H_
