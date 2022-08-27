
#ifndef __UTILS_UTILS_H_
#define __UTILS_UTILS_H_

#include <Eigen/Eigen>
#include <iostream>
#include <unsupported/Eigen/CXX11/Tensor>

#include "io/numpy/array.h"
#include "vp/types.h"

namespace vp
{

template <typename T>
using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

template <typename T, int Dims>
MatrixType<T> numpyArrayToMatrix(const io::numpy::Array<T, Dims>& numpyArray, size_t rows_index = 0,
                                 size_t columns_index = 1)
{
    return Eigen::Map<const MatrixType<T>>(numpyArray.data(), 
                                           numpyArray.dimension(rows_index),
                                           numpyArray.dimension(columns_index));
}


template<size_t Dims>
Vector<Dims+1> to_homogeneous_vector(const Vector<Dims>& vector)
{
    Vector<Dims+1> hvector = Vector<Dims+1>::Ones();
    hvector << vector;
    return hvector;
}

/**
 * @brief converts any vector into a skew symmetric matrix
 * 
 * @param v input 2D vector 
 * @return Matrix3d 
 */
Matrix33 vector_to_skew(const VectorX& v);


}  // namespace vp

#endif  // __UTILS_UTILS_H_