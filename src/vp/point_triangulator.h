#ifndef __VP_POINT_TRIANGULATOR_H_
#define __VP_POINT_TRIANGULATOR_H_

#include <Eigen/Eigen>

#include "types.h"
#include "view.h"

namespace vp
{

class PointTriangulator
{
public:
    Matrix4X run(const std::vector<View::ConstPtr>& views,
                 const std::vector<ConstPtr<Matrix3X>>& correspondences) const;

    Matrix4X run_non_linear(const std::vector<View::ConstPtr>& views,
                            const std::vector<ConstPtr<Matrix3X>>& correspondences,
                            const Matrix4X& points) const;

private:
    Vector4 run_single_point_non_linear(size_t point_index,
                                        const std::vector<View::ConstPtr>& views,
                                        const std::vector<ConstPtr<Matrix3X>>& correspondences,
                                        const Matrix4X& points) const;
};

}  // namespace vp

#endif  // __VP_POINT_TRIANGULATOR_H_