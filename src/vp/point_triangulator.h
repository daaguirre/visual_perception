#ifndef __VP_POINT_TRIANGULATOR_H_
#define __VP_POINT_TRIANGULATOR_H_

#include <Eigen/Eigen>
#include <tuple>

#include "types.h"
#include "view.h"

namespace vp
{

class PointTriangulator
{
    static constexpr double M_MIN_ERROR_DIFF = 1e-3; 

public:
    Matrix4X run(const std::vector<View::ConstPtr>& views,
                 const std::vector<ConstPtr<Matrix3X>>& correspondences) const;

    Matrix4X run_non_linear(const std::vector<View::ConstPtr>& views,
                            const std::vector<ConstPtr<Matrix3X>>& correspondences,
                            const Matrix4X& points, 
                            const size_t max_iterations = 1) const;

private:
    std::tuple<Vector4, double> run_single_point_non_linear(
        size_t point_index, 
        const std::vector<View::ConstPtr>& views,
        const std::vector<ConstPtr<Matrix3X>>& correspondences,
        const Matrix4X& points) const;

    Vector2 calculate_reprojection(const View& view, const Vector4& point);
};

}  // namespace vp

#endif  // __VP_POINT_TRIANGULATOR_H_