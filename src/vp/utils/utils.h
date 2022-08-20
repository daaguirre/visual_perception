
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
HVector<Dims> to_homogeneous_vector(const Vector<Dims>& vector)
{
    HVector<Dims> hvector = HVector<Dims>::Ones();
    hvector << vector;
    return hvector;
}

/**
 * @brief converts 2D vector into a skew symmetric matrix
 * 
 * @param v input 2D vector 
 * @return Matrix3d 
 */
Matrix3d vector_to_skew(const Vector2d& v);


/**
 * @brief converts 3d vector into a skew symmetric matrix
 * 
 * @param v input 3d vector
 * @return Matrix3d 
 */
Matrix3d vector_to_skew(const Vector3d& v);

}  // namespace vp

#endif  // __UTILS_UTILS_H_
