#ifndef __VP_TYPES_H_
#define __VP_TYPES_H_

#include <Eigen/Eigen>
#include <memory>

namespace vp
{

using TransformationMat = Eigen::Matrix3d;

template <size_t Dims>
using Vector = Eigen::Matrix<double, Dims, 1>;

using VectorX = Eigen::Matrix<double, -1, 1>;
using Vector2d = Vector<2>;
using Vector3d = Vector<3>;
using Vector4d = Vector<4>;

// Homogenous vectors
template <size_t Dims>
using HVector = Vector<Dims + 1>;
using HVector2d = HVector<2>;
using HVector3d = HVector<3>;

using Matrix = Eigen::MatrixXd;
using Matrix3d = Eigen::Matrix3d;

struct CameraPose
{
    using MatType = Eigen::Matrix<double, 3, 4>;

    MatType mat;

    CameraPose() : mat(MatType::Identity()) {}

    CameraPose(const TransformationMat& r, const Vector3d& t)
    {
        mat.block<3, 3>(0, 0) = r;
        mat.col(3) = t;
    }

    Eigen::Map<const TransformationMat> R() { return Eigen::Map<const TransformationMat>(&mat(0)); }

    Eigen::Map<const Vector3d> t() { return Eigen::Map<const Vector3d>(&mat.col(3)(0)); }

    Vector3d C() { return -R().transpose() * t(); }
};

}  // namespace vp

#endif  // __VP_TYPES_H_
