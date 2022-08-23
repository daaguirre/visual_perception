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

}  // namespace vp