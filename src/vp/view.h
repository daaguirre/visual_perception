/**
 * @file view.h
 * @author daaguirre
 * @brief
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __VP_VIEW_H_
#define __VP_VIEW_H_

#include "types.h"

namespace vp
{

class View
{
public:
    using Ptr = std::shared_ptr<View>;
    using ConstPtr = std::shared_ptr<const View>;

    View(const Matrix33& k = Matrix33::Identity(), const CameraPose& CameraPose = CameraPose());

    const Matrix33& get_K() const 
    {
        return K;
    }

    const CameraPose& get_camera_pose() const
    {
        return camera_pose;
    }

    Matrix<3, 4> get_P() const
    {
        return K * camera_pose.mat;
    }

private:
    Matrix33 K;
    CameraPose camera_pose;
    // TODO: add image matrix
};

}  // namespace vp

#endif  // __VP_VIEW_H_
