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
    Eigen::MatrixXd run(const Eigen::Matrix3d& K, const Eigen::Vector3d& c1,
                        const Eigen::Matrix3d& R1, const Eigen::Vector3d& c2,
                        const Eigen::Matrix3d& R2, const Eigen::MatrixXd& x1,
                        const Eigen::MatrixXd& x2) const;
    
    Matrix4X run_non_linear(const std::vector<View::ConstPtr>& views, 
                            const std::vector<ConstPtr<Matrix3X>>& correspondences,
                            const Matrix4X& points);

private:
    Vector4d run_single_point_non_linear(size_t point_index, 
                                         const std::vector<View::ConstPtr>& views, 
                                         const std::vector<ConstPtr<Matrix3X>>& correspondences, 
                                         const Matrix4X& points);

};

}  // namespace vp

#endif  // __VP_POINT_TRIANGULATOR_H_