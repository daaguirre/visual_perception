
#include "view.h"


namespace vp
{

View::View(const Matrix33& k, const CameraPose& camera_pose, const cv::Mat& image)
    : K(k), camera_pose(camera_pose), image(image)
{
    
}

}