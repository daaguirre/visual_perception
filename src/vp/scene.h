/**
 * @file scene.h
 * @author daaguirre
 * @brief
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __VP_SCENE_H_
#define __VP_SCENE_H_

#include <vector>

#include "view.h"
#include "types.h"

namespace vp
{

class Scene
{
public:
    Scene();

    Scene& add_view(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints);
    const Matrix4X& get_points(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints);

private:
    std::vector<View::ConstPtr> m_views;
    std::vector<ConstPtr<Matrix3X>> m_correspondences;
    std::unique_ptr<Matrix4X> m_points = nullptr;

    size_t get_num_points();
};

}  // namespace vp
#endif  // __VP_SCENE_H_
