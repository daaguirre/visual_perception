
#include "scene.h"

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <thread>

#include "point_triangulator.h"
#include "utils/enumerate.h"

namespace vp
{

using PointCloud = pcl::PointCloud<pcl::PointXYZRGB>;

vp::Scene::Scene() {}

Scene& vp::Scene::add_view(View::ConstPtr view_ptr, ConstPtr<Matrix3X> keypoints_ptr)
{
    if (!view_ptr)
    {
        throw std::invalid_argument("View ptr is empty.");
    }
    if (!keypoints_ptr)
    {
        throw std::invalid_argument("Keypoints ptr is empty.");
    }

    if (m_views.size() > 0)
    {
        if (std::find(m_views.begin(), m_views.end(), view_ptr) != m_views.end())
        {
            throw std::invalid_argument("View already exists.");
        }

        bool equal = m_correspondences.back()->cols() == keypoints_ptr->cols();
        if (!equal)
        {
            throw std::runtime_error("keypoints size does not match. ");
        }
    }

    m_views.push_back(view_ptr);
    m_correspondences.push_back(keypoints_ptr);
    return *this;
}

const Matrix4X& vp::Scene::get_points() const
{
    if (!m_points.size())
    {
        throw std::runtime_error("Don't exists points for this scene.");
    }

    return m_points;
}

Scene& vp::Scene::triangulate_views()
{
    if (m_views.size() < 2)
    {
        throw std::runtime_error("Not enough views to run triangulation. Minimum views needed 2.");
    }

    PointTriangulator triangulator;
    m_points = triangulator.run(m_views, m_correspondences);
    return *this;
}

Scene& vp::Scene::optimize_points()
{
    if (m_views.size() < 2)
    {
        throw std::runtime_error("Not enough views to run triangulation. Minimum views needed 2.");
    }
    if (!m_points.size())
    {
        throw std::runtime_error("Current scene has no points. Run triangulation first.");
    }

    PointTriangulator triangulator;
    m_points = triangulator.run_non_linear(m_views, m_correspondences, m_points);
    return *this;
}

const Scene& vp::Scene::show_scene() const
{
    for (auto [view_idx, view_ptr] : enumerate(m_views))
    {
        cv::Mat image = view_ptr->get_image().clone();
        MatrixX reprojection_mat = view_ptr->get_P() * m_points;
        for (size_t i = 0; i < reprojection_mat.cols(); ++i)
        {
            Vector3 reprojection = reprojection_mat.col(i);
            reprojection = reprojection / reprojection(2);
            cv::Point reprojection_point(reprojection(0), reprojection(1));
            cv::Point original_point(
                m_correspondences[view_idx]->col(i)(0), m_correspondences[view_idx]->col(i)(1));
            cv::circle(image, reprojection_point, 1, cv::Scalar(0, 255, 0));
            cv::circle(image, original_point, 1, cv::Scalar(255, 0, 0));
            cv::line(image, reprojection_point, original_point, cv::Scalar(0, 0, 255), 1);
        }

        // avoid reusing same window
        static size_t view_id = 0;
        std::string view_name = "view" + std::to_string(view_id);
        cv::imwrite(view_name + ".png", image);

        std::cout << "showing view " << view_name << "\n";
        // cv::imshow(view_name, image);
        // while((cv::waitKey() & 0xEFFFFF) != 27); //27 is the keycode for ESC

        ++view_id;
    }

    return *this;
}

const Scene& Scene::show_scene_point_cloud() const
{
    PointCloud::Ptr cloud = std::make_shared<PointCloud>();
    cloud->points.resize(m_points.cols());
    for (size_t i = 0; i < m_points.cols(); ++i)
    {
        auto& point = cloud->points[i];
        point.getVector3fMap() = m_points.col(i).topRows(3).cast<float>();
        cv::Vec3i avg_pixel(0, 0, 0);
        for (auto [view_idx, view_ptr] : enumerate(m_views))
        {
            const Vector3& pixel_pos = m_correspondences[view_idx]->col(i);
            cv::Vec3b bgr_pixel = view_ptr->get_image().at<cv::Vec3b>(pixel_pos(0), pixel_pos(1));
            avg_pixel += bgr_pixel;
        }
        avg_pixel[0] /= m_views.size();
        avg_pixel[1] /= m_views.size();
        avg_pixel[2] /= m_views.size();
        uint8_t r = (avg_pixel[2]);
        uint8_t g = (avg_pixel[1]);
        uint8_t b = (avg_pixel[0]);

        int32_t rgb = (r << 16) | (g << 8) | b;
        point.rgb = *reinterpret_cast<float*>(&rgb);
    }

    pcl::visualization::PCLVisualizer viewer("Scene cloud");
    viewer.addPointCloud(cloud);
    while (!viewer.wasStopped())
    {
        viewer.spinOnce(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return *this;
}

}  // namespace vp
