
#ifndef __UTILS_UTILS_H_
#define __UTILS_UTILS_H_

#include <Eigen/Eigen>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>

#include "io/numpy/array.h"

namespace vp
{

template <typename T>
using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

template <typename T, int Dims>
MatrixType<T> numpyArrayToMatrix(const io::numpy::Array<T, Dims> &numpyArray,
                                 const Eigen::Index rows, const Eigen::Index cols)
{
    return Eigen::Map<const MatrixType<T>>(numpyArray.data(), rows, cols);
}

}  // namespace vp

#endif  // __UTILS_UTILS_H_
