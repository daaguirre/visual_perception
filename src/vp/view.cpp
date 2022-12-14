
#include "view.h"

namespace vp
{

View::View(const Matrix33& k, const CameraPose& camera_pose, cv::Mat image)
    : m_K(k), m_camera_pose(camera_pose), m_image(image), m_P(m_K * m_camera_pose.mat)
{
}

}  // namespace vp