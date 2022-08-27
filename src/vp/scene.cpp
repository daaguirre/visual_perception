#include "scene.h"

#include <stdexcept>

#include "point_triangulator.h"

namespace vp
{

vp::Scene::Scene() {}

Scene& vp::Scene::add_view(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints)
{
    if (m_views.size() > 0)
    {
        if (std::find(m_views.begin(), m_views.end(), view_ptr) != m_views.end())
        {
            throw std::invalid_argument("View already exists.");
        }

        bool equal = m_correspondences.back()->cols() == keypoints->cols();
        if (!equal)
        {
            throw std::runtime_error("keypoints size does not match. ");
        }
    }

    m_views.push_back(view_ptr);
    m_correspondences.push_back(keypoints);
    return *this;
}

const Matrix4X& vp::Scene::get_points() const
{
    if (!m_points)
    {
        throw std::runtime_error("Don't exists points for this scene.");
    }

    return *m_points;
}

Scene& vp::Scene::triangulate_views()
{
    if(m_views.size() < 2)
    {
        throw std::runtime_error("Not enough views to run triangulation. Minimum views needed 2.");
    }

    PointTriangulator triangulator;
    if(!m_points)
    {
        m_points = std::make_unique<Matrix4X>(); 
    }
    *m_points = triangulator.run(m_views, m_correspondences);
    return *this;
}

Scene& vp::Scene::optimize_points() 
{
    if(m_views.size() < 2)
    {
        throw std::runtime_error("Not enough views to run triangulation. Minimum views needed 2.");
    }
    if(!m_points)
    {
        throw std::runtime_error("Current scene has no points. Run triangulation first.");
    }

    PointTriangulator triangulator;
    if(!m_points)
    {
        m_points = std::make_unique<Matrix4X>(); 
    }

    *m_points = triangulator.run_non_linear(m_views, m_correspondences, *m_points);
    return *this;    
}

size_t vp::Scene::get_num_points()
{
    return m_correspondences.size() > 0 ? m_correspondences[0]->rows() : 0;
}

}  // namespace vp
