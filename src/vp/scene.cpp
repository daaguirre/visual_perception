#include "scene.h"

#include <stdexcept>

namespace vp
{

Scene& vp::Scene::add_view(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints)
{
    if (std::find(m_views.begin(), m_views.end(), view_ptr) == m_views.end())
    {
        throw std::invalid_argument("View already exists.");
    }

    m_views.push_back(view_ptr);
    m_correspondences.push_back(keypoints);
    return *this;
}

const Matrix4X& vp::Scene::get_points(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints) 
{
    if(!m_points)
    {
        throw std::runtime_error("Don't exists points for this scene.");
    }

    return *m_points;
}

size_t vp::Scene::get_num_points()
{
    return m_correspondences.size() > 0 ? m_correspondences[0]->rows() : 0;
}

}  // namespace vp
