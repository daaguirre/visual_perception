#include <unsupported/Eigen/CXX11/Tensor>
#include <iostream>
#include <Eigen/Eigen>
#include "../io/numpy/Array.h"

namespace vp
{

template<typename T>
using  MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

template<typename T, int Dims>
MatrixType<T> numpyArrayToMatrix(const numpy::Array<T, Dims> &numpyArray,const Eigen::Index rows, const Eigen::Index cols)
{
    return Eigen::Map<const MatrixType<T>> (numpyArray.data(), rows,cols);
}

}