#ifndef FUNDAMENTALMATRIXESTIMATOR_H
#define FUNDAMENTALMATRIXESTIMATOR_H

#include "Types.h"
#include <Eigen/Eigen>

namespace vp
{

class FundamentalMatrixEstimator
{
public:
    FundamentalMatrixEstimator();

    Eigen::Matrix3d estimate(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const;

private:

    std::unique_ptr<Eigen::MatrixXd> buildMatrixA(const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2) const;


};

}

#endif