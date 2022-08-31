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

/**
 * @brief This class represents a camera view 
 * Contains the image taken from a camera pose and the intrinsic parameters of the camera.
 */
class View
{
public:
    using Ptr = std::shared_ptr<View>;
    using ConstPtr = std::shared_ptr<const View>;

    View(
        const Matrix33& k = Matrix33::Identity(),
        const CameraPose& CameraPose = CameraPose(),
        cv::Mat mat = cv::Mat());

    /**
     * @brief obtains camera intrinsic parameters
     * 
     * @return const Matrix33& 
     */
    const Matrix33& get_K() const
    {
        return m_K;
    }

    /**
     * @brief Get the camera pose object from where the image was taken
     * 
     * @return const CameraPose& reference to camera pose object
     */
    const CameraPose& get_camera_pose() const
    {
        return m_camera_pose;
    }

    /**
     * @brief obtains perspective matrix K[R t]
     * 
     * @return Matrix<3, 4> 
     */
    const Matrix<3, 4>& get_P() const
    {
        return m_P;
    }

    cv::Mat get_image() const
    {
        return m_image;
    }

private:
    Matrix33 m_K;
    CameraPose m_camera_pose;
    Matrix<3, 4> m_P;
    cv::Mat m_image;
};

}  // namespace vp

#endif  // __VP_VIEW_H_
