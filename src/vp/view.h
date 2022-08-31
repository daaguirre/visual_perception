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

#include <opencv2/opencv.hpp>

#include "types.h"

namespace vp
{

class View
{
public:
    using Ptr = std::shared_ptr<View>;
    using ConstPtr = std::shared_ptr<const View>;

    View(
        const Matrix33& k = Matrix33::Identity(),
        const CameraPose& CameraPose = CameraPose(),
        cv::Mat mat = cv::Mat());

    const Matrix33& get_K() const
    {
        return m_K;
    }

    const CameraPose& get_camera_pose() const
    {
        return m_camera_pose;
    }

    Matrix<3, 4> get_P() const
    {
        return m_K * m_camera_pose.mat;
    }

    cv::Mat get_image() const
    {
        return m_image;
    }

private:
    Matrix33 m_K;
    CameraPose m_camera_pose;
    cv::Mat m_image;
};

}  // namespace vp

#endif  // __VP_VIEW_H_
