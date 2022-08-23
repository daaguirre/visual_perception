

#include "point_triangulator_test.h"

#include <io/numpy/array_io.h>
#include <io/bin/vector_io.h>
#include <vp/point_triangulator.h>
#include <vp/utils/utils.h>

#include <filesystem>

namespace fs = std::filesystem;

TEST_F(PointTriangulatorTest, shouldObtainTriangulation)
{
    Eigen::Matrix3d F;
    F << 2.03779e-07, -3.99522e-06, 0.00255741, 
         5.74935e-06, -8.52717e-08, 0.00148227,
        -0.0037377, -0.00307414, 0.999984;

    Eigen::Matrix3d E;
    E << 0.0133024, -0.260793, 0.0893507,
         0.375297, -0.00556615, 0.58962,
         -0.0984945, -0.652156, 0.0169454;

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

    io::numpy::Array<double, 2> np_K; // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    Eigen::MatrixXd K = vp::numpyArrayToMatrix<double, 2>(np_K);
    std::cout << "K:\n" << K << "\n";

    io::numpy::Array<double, 2> np_R; // numpy array K
    io::numpy::ArrayIO::deserialize(data_R_file_path, np_R);
    Eigen::MatrixXd R2 = vp::numpyArrayToMatrix<double, 2>(np_R);
    std::cout << "R2:\n" << R2 << "\n";

    io::numpy::Array<double, 2> np_C; // numpy array K
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
    std::vector<double> world_points = io::bin::VectorIO::deserializeVector<double>(world_points_file_path); 
    auto world_points_mat = Eigen::Map<Eigen::Matrix<double, 429, 4>>(world_points.data());

    bool equal = world_points_mat.isApprox(points, PRECISION);
    ASSERT_TRUE(equal); 

    // for(size_t i = 0; i < 5; ++i)
    // {
    //     std::cout << points.row(i)<< "\n";
    // }

    // std::vector<double> v(&points(0), points.data()+points.cols()*points.rows());

    // io::bin::VectorIO::serializeVector(v, "/home/diego/dev/VisualPerception/main/tests/resources/world_points.bin");
    // std::cout << "points serialized!\n";

}
