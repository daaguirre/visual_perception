#ifndef FUNDAMENTALMATRIXESTIMATOR_H
#define FUNDAMENTALMATRIXESTIMATOR_H

#include "Types.h"
#include <Eigen/Eigen>


class FundamentalMatrixEstimator
{
public:
    FundamentalMatrixEstimator();

    Ptr<Eigen::Matrix3d> estimate(const Eigen::MatrixX2d& x1, const Eigen::MatrixX2d& x2);

private:

    Ptr<Eigen::MatrixXd> buildMatrixA(const Eigen::MatrixX2d& x1, const Eigen::MatrixX2d& x2);


};

#endif