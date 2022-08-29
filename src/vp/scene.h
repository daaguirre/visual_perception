/**
 * @file scene.h
 * @author daaguirre
 * @brief Defines the Structure from motion scene with multiple views 
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

/**
 * @brief represents a Structure from motion scene with multiple views
 * 
 */
class Scene
{
public:
    Scene();

    /**
     * @brief add view to the scene with its keypoints 
     * 
     * @param view_ptr view to add to the scene 
     * @param keypoints keypoints of the view, must mutch the keypoints of the previously 
     * added views. If it is the first view, the keypoints are not validated
     * @return Scene& return this 
     */
    Scene& add_view(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints);
    
    /**
     * @brief triangulates the stored views (it needs at least 2 views) and generate 
     * a matrix with the 3d points of the scene. The number of points generated match the number 
     * of correspondences/keypoints 
     * 
     * @return Scene& return this
     */
    Scene& triangulate_views();

    /**
     * @brief runs a non linear triangulation with all stored views to reduce the reprojection error
     * and optimize the 3d points of the scene 
     * 
     * @return Scene& 
     */
    Scene& optimize_points();
    
    /**
     * @brief Get the points object with the 3D points of the scene
     * 
     * @param view_ptr 
     * @param keypoints 
     * @return const Matrix4X& 
     */
    const Matrix4X& get_points() const;

    const Scene& visualize_scene() const;

private:
    std::vector<View::ConstPtr> m_views;
    std::vector<ConstPtr<Matrix3X>> m_correspondences;
    std::unique_ptr<Matrix4X> m_points = nullptr;

    size_t get_num_points();
};

}  // namespace vp
#endif  // __VP_SCENE_H_
