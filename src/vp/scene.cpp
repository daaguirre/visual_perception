
#include <stdexcept>
#include <opencv2/opencv.hpp>

#include "scene.h"
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

    *m_points = triangulator.run_non_linear(m_views, m_correspondences, *m_points, 10);
    return *this;    
}

size_t vp::Scene::get_num_points()
{
    return m_correspondences.size() > 0 ? m_correspondences[0]->rows() : 0;
}

const Scene& vp::Scene::visualize_scene() const 
{
    size_t view_idx = 0;
    for(const auto& view_ptr : m_views)
    {
        cv::Mat image = view_ptr->get_image().clone();
        MatrixX reprojection_mat = view_ptr->get_P() * (*m_points);
        for(size_t i = 0; i < reprojection_mat.cols(); ++i)
        {
            Vector3 reprojection = reprojection_mat.col(i);
            reprojection = reprojection / reprojection(2);
            cv::Point reprojection_point(reprojection(0), reprojection(1));
            cv::Point original_point(m_correspondences[view_idx]->col(i)(0), 
                                     m_correspondences[view_idx]->col(i)(1));
            cv::circle(image, reprojection_point, 1, cv::Scalar(0, 255, 0));                   
            cv::circle(image, original_point, 1, cv::Scalar(255, 0, 0));                   
            cv::line(image, reprojection_point, original_point, cv::Scalar(0, 0, 255), 1);
        }

        static size_t view_id = 0;
        std::string view_name = "view" + std::to_string(view_id);
        ++view_id;
        cv::imwrite(view_name + ".png", image);
        
        std::cout << "showing view " << view_name << "\n"; 
        // cv::imshow(view_name, image);
        // while((cv::waitKey() & 0xEFFFFF) != 27); //27 is the keycode for ESC

        ++view_idx; 
    }

    return *this;
}

}  // namespace vp

