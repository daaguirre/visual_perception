
#include "view.h"


vp::View::View(const Matrix33& k, const CameraPose& camera_pose)
    : K(k), camera_pose(camera_pose)
{
    
}
