

#include "point_triangulator_test.h"

#include <io/bin/vector_io.h>
#include <io/numpy/array_io.h>
#include <vp/point_triangulator.h>
#include <vp/utils/utils.h>
#include <vp/view.h>

#include <filesystem>

namespace fs = std::filesystem;

TEST_F(PointTriangulatorTest, should_obtain_triangulation)
{
    std::string data_x1_file_path = (RESOURCES_DIR / "data_x1.npy").string();
    std::string data_x2_file_path = (RESOURCES_DIR / "data_x2.npy").string();
    std::string data_K_file_path = (RESOURCES_DIR / "data_K.npy").string();
    std::string data_R_file_path = (RESOURCES_DIR / "data_R.npy").string();
    std::string data_C_file_path = (RESOURCES_DIR / "data_C.npy").string();

    io::numpy::Array<double, 2> np_x1;
    io::numpy::ArrayIO::deserialize(data_x1_file_path, np_x1);
    Eigen::MatrixXd x1 = vp::numpyArrayToMatrix<double, 2>(np_x1);

    io::numpy::Array<double, 2> np_x2;
    io::numpy::ArrayIO::deserialize(data_x2_file_path, np_x2);
    Eigen::MatrixXd x2 = vp::numpyArrayToMatrix<double, 2>(np_x2);

    io::numpy::Array<double, 2> np_K;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    Eigen::MatrixXd K = vp::numpyArrayToMatrix<double, 2>(np_K);
    std::cout << "K:\n" << K << "\n";

    io::numpy::Array<double, 2> np_R;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_R_file_path, np_R);
    Eigen::MatrixXd R2 = vp::numpyArrayToMatrix<double, 2>(np_R);
    std::cout << "R2:\n" << R2 << "\n";

    io::numpy::Array<double, 2> np_C;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_C_file_path, np_C);
    Eigen::MatrixXd C2 = vp::numpyArrayToMatrix<double, 2>(np_C);
    std::cout << "C2:\n" << C2 << "\n";

    auto R1 = Eigen::Matrix3d::Identity();
    auto C1 = Eigen::Vector3d::Zero();

    vp::PointTriangulator triangulator;
    Eigen::MatrixXd points = triangulator.run(K, C1, R1, C2, R2, x1, x2);

    ASSERT_EQ(429, points.rows());
    ASSERT_EQ(4, points.cols());

    const std::string world_points_file_path = (RESOURCES_DIR / "world_points.bin").string();
    std::vector<double> world_points =
        io::bin::VectorIO::deserializeVector<double>(world_points_file_path);
    auto world_points_mat = Eigen::Map<Eigen::Matrix<double, 429, 4>>(world_points.data());

    bool equal = world_points_mat.isApprox(points, PRECISION);
    ASSERT_TRUE(equal);

    // for(size_t i = 0; i < 5; ++i)
    // {
    //     std::cout << points.row(i)<< "\n";
    // }

    // std::vector<double> v(&points(0), points.data()+points.cols()*points.rows());

    // io::bin::VectorIO::serializeVector(v,
    // "/home/diego/dev/VisualPerception/main/tests/resources/world_points.bin"); std::cout <<
    // "points serialized!\n";
}

TEST_F(PointTriangulatorTest, should_obtain_non_linear_triangulation)
{
    std::string data_x1_file_path = (RESOURCES_DIR / "data_x1.npy").string();
    std::string data_x2_file_path = (RESOURCES_DIR / "data_x2.npy").string();
    std::string data_x3_file_path = (RESOURCES_DIR / "data_x3.npy").string();
    std::string data_K_file_path = (RESOURCES_DIR / "data_K.npy").string();
    std::string data_R_file_path = (RESOURCES_DIR / "data_R.npy").string();
    std::string data_C_file_path = (RESOURCES_DIR / "data_C.npy").string();

    io::numpy::Array<double, 2> np_x1;
    io::numpy::ArrayIO::deserialize(data_x1_file_path, np_x1);
    Eigen::MatrixXd x1 = vp::numpyArrayToMatrix<double, 2>(np_x1);
    x1.transposeInPlace();
    x1.conservativeResize(x1.rows() + 1, Eigen::NoChange);
    x1.row(2).setOnes();

    io::numpy::Array<double, 2> np_x2;
    io::numpy::ArrayIO::deserialize(data_x2_file_path, np_x2);
    Eigen::MatrixXd x2 = vp::numpyArrayToMatrix<double, 2>(np_x2);
    x2.transposeInPlace();
    x2.conservativeResize(x2.rows() + 1, Eigen::NoChange);
    x2.row(2).setOnes();

    io::numpy::Array<double, 2> np_x3;
    io::numpy::ArrayIO::deserialize(data_x3_file_path, np_x3);
    Eigen::MatrixXd x3 = vp::numpyArrayToMatrix<double, 2>(np_x3);
    x3.transposeInPlace();
    x3.conservativeResize(x3.rows() + 1, Eigen::NoChange);
    x3.row(2).setOnes();

    io::numpy::Array<double, 2> np_K;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    Eigen::MatrixXd K = vp::numpyArrayToMatrix<double, 2>(np_K);
    std::cout << "K:\n" << K << "\n";

    io::numpy::Array<double, 2> np_R;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_R_file_path, np_R);
    Eigen::MatrixXd R2 = vp::numpyArrayToMatrix<double, 2>(np_R);
    std::cout << "R2:\n" << R2 << "\n";

    io::numpy::Array<double, 2> np_C;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_C_file_path, np_C);
    Eigen::MatrixXd C2 = vp::numpyArrayToMatrix<double, 2>(np_C);
    std::cout << "C2:\n" << C2 << "\n";

    auto R1 = Eigen::Matrix3d::Identity();
    auto C1 = Eigen::Vector3d::Zero();

    const std::string world_points_file_path = (RESOURCES_DIR / "world_points.bin").string();
    std::vector<double> world_points_vector =
        io::bin::VectorIO::deserializeVector<double>(world_points_file_path);
    Eigen::Matrix<double, 429, 4> world_points =
        Eigen::Map<const Eigen::Matrix<double, 429, 4>>(world_points_vector.data(), 429, 4);
    Eigen::Matrix<double, 4, 429> points = world_points.transpose();

    vp::CameraPose camera_pose3;
    camera_pose3.mat << 0.98967106597, 0.0393964, 0.13783724, 1.492793556, -0.0339231106,
        0.99854846, -0.0418354, -0.27435807, -0.13928533, 0.0367275, 0.989571, -1.68229621;

    auto t1 = -R1 * C1;
    auto t2 = -R2 * C2;
    auto view1_ptr = std::make_shared<vp::View>(K, vp::CameraPose(R1, C1));
    auto view2_ptr = std::make_shared<vp::View>(K, vp::CameraPose(R2, t2));
    auto view3_ptr = std::make_shared<vp::View>(K, camera_pose3);
    std::vector<vp::View::ConstPtr> views = {view1_ptr, view2_ptr, view3_ptr};
    std::vector<vp::ConstPtr<vp::Matrix3X>> correspondences = {std::make_shared<vp::Matrix3X>(x1),
                                                               std::make_shared<vp::Matrix3X>(x2),
                                                               std::make_shared<vp::Matrix3X>(x3)};


    vp::PointTriangulator triangulator;
    Eigen::Matrix<double, 4, 429> opt_points = triangulator.run_non_linear(views, correspondences, points);

    // std::vector<double> v(&opt_points(0), opt_points.data()+opt_points.cols()*opt_points.rows());
    // io::bin::VectorIO::serializeVector(v, "/home/diego/dev/VisualPerception/main/tests/resources/opt_points.bin"); 
    // std::cout <<"points serialized!\n";

    const std::string opt_points_file_path = (RESOURCES_DIR / "opt_points.bin").string();
    std::vector<double> opt_points_vector = io::bin::VectorIO::deserializeVector<double>(opt_points_file_path);
    auto expected_opt_points = Eigen::Map<Eigen::Matrix<double, 4, 429>>(opt_points_vector.data());

    bool equal = expected_opt_points.isApprox(opt_points, PRECISION);
    ASSERT_TRUE(equal);
}
