#include "pnp_algorithm.h"

#include "utils/utils.h"

namespace vp
{

CameraPose PNPAlgorithm::run(
    const Matrix4X& world_points,
    const Matrix3X& pixels,
    const Matrix33& K)
{
    MatrixX img_points = factor_out_intrinsic_parameters(K, pixels);
    auto h_zero = Matrix<1, 4>::Zero();
    size_t num_points = world_points.cols();
    MatrixX A(3 * num_points, 12);

    for (size_t i = 0; i < num_points; ++i)
    {
        const Vector3& img_point = img_points.col(i);
        const Matrix<1, 4>& h_world_point = world_points.col(i).transpose();
        Matrix<1, 4> u_world_point = img_point(0) * h_world_point;
        Matrix<1, 4> v_world_point = img_point(1) * h_world_point;
        Eigen::MatrixXd partial_A(3, 12);
        partial_A << h_zero, -h_world_point, v_world_point, h_world_point, h_zero, -u_world_point,
            -v_world_point, u_world_point, h_zero;
        A.block<3, 12>(i * 3, 0) = partial_A;
    }

    Eigen::BDCSVD<MatrixX> svd(A, Eigen::ComputeFullV);
    const MatrixX& V = svd.matrixV();
    auto camera_pose_mat =
        Eigen::Map<const Eigen::Matrix<double, 3, 4, Eigen::RowMajor>>(V.col(11).data());
    const Vector3& t = camera_pose_mat.col(3);

    Eigen::BDCSVD<MatrixX> svd2(
        camera_pose_mat.leftCols(3), Eigen::ComputeFullU | Eigen::ComputeFullV);
    const MatrixX& V2 = svd2.matrixV();
    const MatrixX& U2 = svd2.matrixU();
    MatrixX R = U2 * V2.transpose();
    double det_R = R.determinant();
    Vector3 tc = det_R * (t / svd2.singularValues()(0));

    CameraPose camera_pose(R, tc);
    return camera_pose;
}

MatrixX vp::PNPAlgorithm::factor_out_intrinsic_parameters(const Matrix33& K, const Matrix3X& pixels)
{
    MatrixX img_points = K.inverse() * pixels;
    return img_points;
}

}  // namespace vp
