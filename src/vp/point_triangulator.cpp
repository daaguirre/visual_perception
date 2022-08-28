#include "point_triangulator.h"

#include "utils/utils.h"

namespace vp
{

// double PointTriangulator::M_MIN_ERROR_DIFF = 1e-3;

Matrix4X PointTriangulator::run(const std::vector<View::ConstPtr>& views,
                                const std::vector<ConstPtr<Matrix3X>>& correspondences) const
{
    size_t num_points = correspondences[0]->cols();
    Matrix4X points_3d(4, num_points);
    for (size_t point_idx = 0; point_idx < num_points; ++point_idx)
    {
        MatrixX a(views.size() * 3, 4);
        for (size_t view_idx = 0; view_idx < views.size(); ++view_idx)
        {
            const Vector3& pixel = correspondences[view_idx]->col(point_idx);
            Matrix33 skew_mat = vector_to_skew(pixel);
            MatrixX p = skew_mat * views[view_idx]->get_P();
            a.block<3, 4>(view_idx * 3, 0) = p;
        }

        Eigen::BDCSVD<Eigen::MatrixXd> svd(a, Eigen::ComputeFullV);
        const MatrixX& V = svd.matrixV();
        Vector4 point = V.col(V.cols() - 1);
        point /= point(3);
        points_3d.col(point_idx) = point;
    }
    return points_3d;
}

Matrix4X vp::PointTriangulator::run_non_linear(
    const std::vector<View::ConstPtr>& views,
    const std::vector<ConstPtr<Matrix3X>>& correspondences, 
    const Matrix4X& points,
    const size_t max_iterations
) const
{
    const size_t num_points = points.cols();
    Matrix4X opt_points(points.rows(), points.cols());
    const Matrix4X* previous_points = &points;
    double previous_error = 1e99;
    for(size_t it = 0; it < max_iterations; ++it)
    {
        double total_error = 0;
        for (size_t i = 0; i < num_points; ++i)
        {
            auto [opt_point, error] = run_single_point_non_linear(i, views, correspondences, *previous_points);
            opt_points.col(i) = opt_point;
            total_error += error;
        }

        std::cout << "Total error: " << total_error << "\n";
        double error_diff = previous_error - total_error;
        if(error_diff < M_MIN_ERROR_DIFF)
        {
            std::cout << "Reached min error at iteration " << it+1 << "\n";
            break;
        } 
        previous_points = &opt_points;
        previous_error = total_error;
    }
    return opt_points;
}

std::tuple<Vector4, double> vp::PointTriangulator::run_single_point_non_linear(
    size_t point_idx, const std::vector<View::ConstPtr>& views,
    const std::vector<ConstPtr<Matrix3X>>& correspondences, const Matrix4X& points) const
{
    const Vector4& current_point = points.col(point_idx);

    VectorX b(views.size() * 2);
    VectorX f(b.rows());
    MatrixX J(b.rows(), 3);
    for (size_t view_idx = 0; view_idx < views.size(); ++view_idx)
    {
        const View& view = *views[view_idx];
        size_t row_idx0 = view_idx * 2;
        size_t row_idx1 = view_idx * 2 + 1;
        b.block<2, 1>(row_idx0, 0) = correspondences[view_idx]->col(point_idx).topRows(2);
        Vector3 reprojection = view.get_P() * current_point;
        const double u = reprojection(0);
        const double v = reprojection(1);
        const double w = reprojection(2);
        const double w2 = w * w;
        reprojection = reprojection / w;
        f.block<2, 1>(row_idx0, 0) = reprojection.topRows(2);
        auto K = view.get_K();
        auto R = view.get_camera_pose().R();
        Matrix33 p = K * R;
        const Vector3& du_dx = p.row(0);
        const Vector3& dv_dx = p.row(1);
        const Vector3& dw_dx = p.row(2);
        Vector3 df_dx0 = (w * du_dx - u * dw_dx) / w2;
        Vector3 df_dx1 = (w * dv_dx - v * dw_dx) / w2;
        J.row(row_idx0) = df_dx0.transpose();
        J.row(row_idx1) = df_dx1.transpose();
    }

    MatrixX j_tilde = (J.transpose() * J).inverse() * J.transpose();
    MatrixX delta_x = j_tilde * (b - f);
    Vector4 opt_point = Vector4::Ones();
    opt_point.topRows(3) = current_point.topRows(3) + delta_x;

    VectorX new_f(b.rows());
    size_t view_idx = 0;
    for (auto view_ptr : views)
    {
        Vector3 reprojection = view_ptr->get_P() * opt_point;
        reprojection = reprojection / reprojection(2);
        new_f.block<2, 1>(view_idx * 2, 0) = reprojection.topRows(2);
        ++view_idx;
    }
    double error = (new_f - b).sum();

    return {opt_point, error};
}

}  // namespace vp
