#include "point_triangulator.h"

#include "utils/utils.h"

namespace vp
{

Eigen::MatrixXd PointTriangulator::run(const Eigen::Matrix3d& K, const Eigen::Vector3d& c1,
                                       const Eigen::Matrix3d& R1, const Eigen::Vector3d& c2,
                                       const Eigen::Matrix3d& R2, const Eigen::MatrixXd& x1,
                                       const Eigen::MatrixXd& x2) const
{
    Eigen::Vector3d t1 = -R1 * c1;
    Eigen::Vector3d t2 = -R2 * c2;

    Eigen::MatrixXd P1(R1.rows(), R1.cols() + 1);
    P1 << R1, t1;
    P1 = K * P1;

    Eigen::MatrixXd P2(R2.rows(), R2.cols() + 1);
    P2 << R2, t2;
    P2 = K * P2;

    size_t num_points = x1.rows();
    Eigen::MatrixXd points_3d(num_points, 4);
    for (size_t i = 0; i < num_points; ++i)
    {
        Eigen::Matrix3d skew_mat1 = vector_to_skew(x1.row(i));
        Eigen::MatrixXd p1 = skew_mat1 * P1;
        Eigen::Matrix3d skew_mat2 = vector_to_skew(x2.row(i));
        Eigen::MatrixXd p2 = skew_mat2 * P2;
        Eigen::MatrixXd a(6, 4);
        a << p1, p2;
        Eigen::BDCSVD<Eigen::MatrixXd> svd(a, Eigen::ComputeThinU | Eigen::ComputeThinV);
        const Eigen::MatrixXd& V = svd.matrixV();
        Eigen::Vector4d point = V.col(V.cols() - 1);
        point /= point(3);
        points_3d.row(i) = point;
    }
    return points_3d;
}

Matrix4X vp::PointTriangulator::run_non_linear(
    const std::vector<View::ConstPtr>& views,
    const std::vector<ConstPtr<Matrix3X>>& correspondences, const Matrix4X& points)
{
    const size_t num_points = points.cols();
    Matrix4X opt_points(points.rows(), points.cols());
    for (size_t i = 0; i < num_points; ++i)
    {
        opt_points.col(i) = run_single_point_non_linear(i, views, correspondences, points);
    }
    return opt_points;
}

Vector4d vp::PointTriangulator::run_single_point_non_linear(
    size_t point_index, const std::vector<View::ConstPtr>& views,
    const std::vector<ConstPtr<Matrix3X>>& correspondences, const Matrix4X& points)
{
    VectorX b(views.size() * 2);
    VectorX f(b.rows());
    MatrixX J(views.size() * 2, 3);
    for (size_t i = 0; i < views.size(); ++i)
    {
        const View& view = *views[i];
        b.block<2, 1>(i * 2, 0) = correspondences[i]->col(point_index).topRows(2);
        Vector3d reprojection = view.get_P() * points.col(point_index);
        const double u = reprojection(0);
        const double v = reprojection(1);
        const double w = reprojection(2);
        const double w2 = w * w;
        reprojection = reprojection / w;
        f.block<2, 1>(i * 2, 0) = reprojection.topRows(2);
        auto K = view.get_K();
        auto R = view.get_camera_pose().R();
        Matrix33 p = K * R;
        const Vector3d& du_dx = p.row(0);
        const Vector3d& dv_dx = p.row(1);
        const Vector3d& dw_dx = p.row(2);
        Vector3d df_dx0 = (w * du_dx - u * dw_dx) / w2;
        Vector3d df_dx1 = (w * dv_dx - v * dw_dx) / w2;
        J.row(i * 2) = df_dx0.transpose();
        J.row(i * 2 + 1) = df_dx1.transpose();
    }

    MatrixX j_tilde = (J.transpose() * J).inverse() * J.transpose();
    MatrixX delta_x = j_tilde * (b - f);
    const size_t num_its = 1;
    Vector4d opt_point = Vector4d::Ones();
    for (size_t i = 0; i < num_its; ++i)
    {
        opt_point.topRows(3) = points.col(point_index).topRows(3) + delta_x;
    }

    return opt_point;
}

}  // namespace vp
