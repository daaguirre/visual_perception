#include "pnp_algorithm_test.h"

#include <io/bin/vector_io.h>
#include <io/numpy/array_io.h>
#include <vp/pnp_algorithm.h>
#include <vp/utils/utils.h>

#include <filesystem>

TEST_F(PNPAlgorithmTest, shouldObtainCameraPose)
{
    const std::string data_x3_file_path = (RESOURCES_DIR / "data_x3.npy").string();
    io::numpy::Array<double, 2> np_x3;
    io::numpy::ArrayIO::deserialize(data_x3_file_path, np_x3);
    Eigen::MatrixXd x3 = vp::numpyArrayToMatrix<double, 2>(np_x3);

    const std::string data_K_file_path = (RESOURCES_DIR / "data_K.npy").string();
    io::numpy::Array<double, 2> np_K;  // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    Eigen::MatrixXd K = vp::numpyArrayToMatrix<double, 2>(np_K);

    const std::string world_points_file_path = (RESOURCES_DIR / "world_points.bin").string();
    std::vector<double> world_points = io::bin::VectorIO::deserializeVector<double>(world_points_file_path); 
    auto world_points_mat = Eigen::Map<Eigen::Matrix<double, 429, 4>>(world_points.data());

    vp::PNPAlgorithm pnp;
    vp::CameraPose camera_pose = pnp.run(world_points_mat, x3, K);

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