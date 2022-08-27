#ifndef __VP_TYPES_H_
#define __VP_TYPES_H_

#include <Eigen/Eigen>
#include <memory>

namespace vp
{

template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
using ConstPtr = std::shared_ptr<const T>;

template <int Rows = -1, int Cols = -1>
using Matrix = Eigen::Matrix<double, Rows, Cols>;
using MatrixX = Matrix<-1, -1>;
using Matrix3X = Matrix<3, -1>;
using Matrix4X = Matrix<4, -1>;
using Matrix33 = Matrix<3, 3>;

// collumn vectors
using VectorX = Matrix<-1, 1>;

template <size_t Dims>
using Vector = Matrix<Dims, 1>;
using Vector2d = Vector<2>;
using Vector3d = Vector<3>;
using Vector4d = Vector<4>;

struct CameraPose
{
    using Ptr = std::shared_ptr<CameraPose>;
    using MatType = Matrix<3, 4>;

    MatType mat;

    CameraPose() : mat(MatType::Identity()) {}

    CameraPose(const Matrix33& r, const Vector3d& t)
    {
        mat.block<3, 3>(0, 0) = r;
        mat.col(3) = t;
    }

    Eigen::Map<const Matrix33> R() const { return Eigen::Map<const Matrix33>(&mat(0)); }

    Eigen::Ref<const Vector3d> t() const { return mat.col(3); }

    Vector3d C() const { return -R().transpose() * t(); }
};

}  // namespace vp

#endif  // __VP_TYPES_H_
