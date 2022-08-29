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
#include <opencv2/opencv.hpp>

namespace vp
{

class View
{
public:
    using Ptr = std::shared_ptr<View>;
    using ConstPtr = std::shared_ptr<const View>;

    View(const Matrix33& k = Matrix33::Identity(), 
         const CameraPose& CameraPose = CameraPose(),
         const cv::Mat& mat = cv::Mat());

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

    const cv::Mat& get_image() const
    {
        return image;
    }

private:
    Matrix33 K;
    CameraPose camera_pose;
    cv::Mat image;
};

}  // namespace vp

#endif  // __VP_VIEW_H_
