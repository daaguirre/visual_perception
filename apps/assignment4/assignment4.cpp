#include "assignment4.h"

#include <Eigen/Eigen>
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "io/numpy/array_io.h"
#include "vp/essential_matrix_estimator.h"
#include "vp/fundamental_matrix_estimator.h"
#include "vp/pnp_algorithm.h"
#include "vp/scene.h"
#include "vp/types.h"
#include "vp/utils/utils.h"

void assignment4()
{
    AssignmentData lab_data = load_data();
    vp::Matrix33 fundamental_matrix = estimate_fundamental_matrix(lab_data);
    estimate_essential_matrix(fundamental_matrix, lab_data.K);

    run_sfm_scene(lab_data);

    std::cout << "Visual Perception assignment 4 done!\n";
}

vp::ConstPtr<vp::Matrix3X> load_keypoints(const std::string& file_name)
{
    std::string data_file_path = (lab_data_dir_path / file_name).string();
    io::numpy::Array<double, 2> np_array;
    io::numpy::ArrayIO::deserialize(data_file_path, np_array);
    vp::MatrixX x1 = vp::numpy_array_to_matrix<double, 2>(np_array);
    x1.transposeInPlace();
    x1.conservativeResize(x1.rows() + 1, Eigen::NoChange);
    x1.row(2).setOnes();
    return std::make_shared<vp::Matrix3X>(std::move(x1));
}

AssignmentData load_data()
{
    AssignmentData lab_data;
    lab_data.x1 = load_keypoints("data_x1.npy");
    lab_data.x2 = load_keypoints("data_x2.npy");
    lab_data.x3 = load_keypoints("data_x3.npy");

    std::string data_K_file_path = (lab_data_dir_path / "data_K.npy").string();
    io::numpy::Array<double, 2> np_K;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    lab_data.K = vp::numpy_array_to_matrix<double, 2>(np_K);
    std::cout << "K:\n" << lab_data.K << "\n";

    std::string data_R_file_path = (lab_data_dir_path / "data_R.npy").string();
    io::numpy::Array<double, 2> np_R;
    io::numpy::ArrayIO::deserialize(data_R_file_path, np_R);
    lab_data.R2 = vp::numpy_array_to_matrix<double, 2>(np_R);
    std::cout << "R2:\n" << lab_data.R2 << "\n";

    std::string data_C_file_path = (lab_data_dir_path / "data_C.npy").string();
    io::numpy::Array<double, 2> np_C;
    io::numpy::ArrayIO::deserialize(data_C_file_path, np_C);
    lab_data.C2 = vp::numpy_array_to_matrix<double, 2>(np_C);
    std::cout << "C2:\n" << lab_data.C2 << "\n";

    std::string img1_path = (lab_data_dir_path / "img1.png").string();
    lab_data.img1 = cv::imread(img1_path);
    std::string img2_path = (lab_data_dir_path / "img2.png").string();
    lab_data.img2 = cv::imread(img2_path);
    std::string img3_path = (lab_data_dir_path / "img3.png").string();
    lab_data.img3 = cv::imread(img3_path);

    return lab_data;
}

vp::Matrix33 estimate_fundamental_matrix(const AssignmentData& lab_data)
{
    vp::FundamentalMatrixEstimator fundamental_mat_estimator;
    vp::Matrix33 fundamental_mat = fundamental_mat_estimator.estimate(*lab_data.x1, *lab_data.x2);
    std::cout << "Fundamental Matrix: \n" << fundamental_mat << "\n";
    return fundamental_mat;
}

void estimate_essential_matrix(const vp::Matrix33& fundamental_matrix, const vp::Matrix33& K)
{
    vp::EssentialMatrixEstimator essential_mat_estimator;
    vp::Matrix33 essential_mat =
        essential_mat_estimator.estimate_from_fundamental_matrix(fundamental_matrix, K);
    std::cout << "Essential Matrix: \n" << essential_mat << "\n";
}

void run_sfm_scene(const AssignmentData& lab_data)
{
    auto R1 = vp::Matrix33::Identity();
    auto C1 = vp::Vector3::Zero();

    auto t1 = -R1 * C1;
    auto t2 = -lab_data.R2 * lab_data.C2;
    vp::CameraPose cp1(R1, t1);
    auto view1_ptr = std::make_shared<vp::View>(lab_data.K, cp1, lab_data.img1);

    vp::CameraPose cp2(lab_data.R2, t2);
    auto view2_ptr = std::make_shared<vp::View>(lab_data.K, cp2, lab_data.img2);

    // clang-format off
    vp::Scene scene;
    scene.add_view(view1_ptr, lab_data.x1)
         .add_view(view2_ptr, lab_data.x2)
         .triangulate_views();
    // clang-format on

    const vp::Matrix4X& points = scene.get_points();
    // print first 10 points as row vectors
    std::cout << "Points linear triangulation: \n"
              << points.block<3, 10>(0, 0).transpose() << "\n\n";

    vp::PNPAlgorithm pnp;
    vp::CameraPose camera_pose3 = pnp.run(points, *lab_data.x3, lab_data.K);
    auto view3_ptr = std::make_shared<vp::View>(lab_data.K, camera_pose3, lab_data.img3);

    std::cout << "PNP done: \n" << camera_pose3.mat << "\n\n";

    // clang-format off
    scene.add_view(view3_ptr, lab_data.x3)
         .show_scene();

    scene.optimize_points()
         .show_scene()
         .show_scene_point_cloud();
    // clang-format on

    // print first 10 points as row vectors
    std::cout << "Final points: \n" << points.block<3, 10>(0, 0).transpose() << "\n\n";
}
