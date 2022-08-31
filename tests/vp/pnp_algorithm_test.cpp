#include "pnp_algorithm_test.h"

#include <io/bin/vector_io.h>
#include <io/numpy/array_io.h>
#include <vp/pnp_algorithm.h>
#include <vp/utils/utils.h>

#include <filesystem>

TEST_F(PNPAlgorithmTest, should_obtain_camera_pose)
{
    std::string data_x3_file_path = (RESOURCES_DIR / "data_x3.npy").string();
    io::numpy::Array<double, 2> np_x3;
    io::numpy::ArrayIO::deserialize(data_x3_file_path, np_x3);
    vp::MatrixX x3 = vp::numpy_array_to_matrix<double, 2>(np_x3);
    x3.transposeInPlace();
    x3.conservativeResize(x3.rows() + 1, Eigen::NoChange);
    x3.row(2).setOnes();

    std::string data_K_file_path = (RESOURCES_DIR / "data_K.npy").string();
    io::numpy::Array<double, 2> np_K;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    vp::MatrixX K = vp::numpy_array_to_matrix<double, 2>(np_K);

    const std::string world_points_file_path = (RESOURCES_DIR / "world_points.bin").string();
    std::vector<double> world_points_vector =
        io::bin::VectorIO::deserializeVector<double>(world_points_file_path);
    vp::Matrix<429, 4> world_points =
        Eigen::Map<const vp::Matrix<429, 4>>(world_points_vector.data(), 429, 4);
    vp::Matrix<4, 429> points = world_points.transpose();

    vp::PNPAlgorithm pnp;
    vp::CameraPose camera_pose = pnp.run(points, x3, K);

    std::cout << "PNP algorithm runned!\n";
    std::cout << "Matrix:\n";
    std::cout << camera_pose.mat << "\n";
    std::cout << "R:\n";
    std::cout << camera_pose.R() << "\n";
    std::cout << "t:\n";
    std::cout << camera_pose.t() << "\n";
    std::cout << "C:\n";
    std::cout << camera_pose.C() << "\n";

    vp::CameraPose expected_camera_pose;
    expected_camera_pose.mat << 0.98967106597, 0.0393964, 0.13783724, 1.492793556,
                                -0.0339231106, 0.99854846, -0.0418354, -0.27435807,
                                -0.13928533, 0.0367275, 0.989571, -1.68229621;
    bool equal = expected_camera_pose.mat.isApprox(camera_pose.mat, PRECISION);
    ASSERT_TRUE(equal);
}