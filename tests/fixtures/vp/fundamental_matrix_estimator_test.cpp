
#include "fundamental_matrix_estimator_test.h"

#include <io/numpy/array_io.h>
#include <vp/fundamental_matrix_estimator.h>
#include <vp/utils/utils.h>
#include <vp/types.h>

#include <filesystem>

namespace fs = std::filesystem;

TEST_F(FundamentalMatrixEstimatorTest, shouldObtainFundamentalMatrix)
{
    std::string data_x1_file_path = (RESOURCES_DIR / "data_x1.npy").string();
    io::numpy::Array<double, 2> np_x1;
    io::numpy::ArrayIO::deserialize(data_x1_file_path, np_x1);
    vp::MatrixX x1 = vp::numpy_array_to_matrix<double, 2>(np_x1);
    x1.transposeInPlace();
    x1.conservativeResize(x1.rows() + 1, Eigen::NoChange);
    x1.row(2).setOnes();

    std::string data_x2_file_path = (RESOURCES_DIR / "data_x2.npy").string();
    io::numpy::Array<double, 2> np_x2;
    io::numpy::ArrayIO::deserialize(data_x2_file_path, np_x2);
    vp::MatrixX x2 = vp::numpy_array_to_matrix<double, 2>(np_x2);
    x2.transposeInPlace();
    x2.conservativeResize(x2.rows() + 1, Eigen::NoChange);
    x2.row(2).setOnes();
    std::cout << "Input data read!!\n";

    std::cout << "Estimating Fundamental matrix!\n";
    vp::FundamentalMatrixEstimator estimator;
    vp::Matrix33 F = estimator.estimate(x1, x2);
    std::cout << F << "\n";

    vp::Matrix33 expected_matrix;
    expected_matrix << 2.03779e-07, -3.99522e-06, 0.00255741,
                       5.74935e-06, -8.52717e-08, 0.00148227,
                       -0.0037377, -0.00307414, 0.999984;
    bool equal = expected_matrix.isApprox(F, PRECISION);
    ASSERT_TRUE(equal);
}
