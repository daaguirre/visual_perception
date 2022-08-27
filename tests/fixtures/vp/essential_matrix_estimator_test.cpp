#include "essential_matrix_estimator_test.h"

#include <io/numpy/array_io.h>
#include <vp/essential_matrix_estimator.h>
#include <vp/utils/utils.h>
#include <vp/types.h>

TEST_F(EssentialMatrixEstimatorTest, shouldObtainEssentialMatrix)
{
    vp::Matrix33 F;
    F << 2.03779e-07, -3.99522e-06, 0.00255741, 
         5.74935e-06, -8.52717e-08, 0.00148227,
        -0.0037377, -0.00307414, 0.999984;
    
    std::string data_K_file_path = (RESOURCES_DIR / "data_K.npy").string();

    io::numpy::Array<double, 2> np_K; // numpy array K
    io::numpy::ArrayIO::deserialize(data_K_file_path, np_K);
    Eigen::MatrixXd K = vp::numpy_array_to_matrix<double, 2>(np_K);
    std::cout << "\n" << K << "\n";

    vp::EssentialMatrixEstimator estimator;
    Eigen::MatrixXd E = estimator.estimateFromFundamentalMatrix(F, K);
    std::cout << "\n" << "Essential matrix:" << "\n";
    std::cout << "\n" << E << "\n";
    vp::Matrix33 expected_matrix;
    expected_matrix << 0.0133024, -0.260793, 0.0893507,
                       0.375297, -0.00556615, 0.58962,
                       -0.0984945, -0.652156, 0.0169454;

    bool equal = expected_matrix.isApprox(E, PRECISION);
    ASSERT_TRUE(equal);
}
