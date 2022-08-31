//
// Created by Diego Aguirre on 3/12/20.
//
#ifndef __IO_NUMPY_ARRAY_H_
#define __IO_NUMPY_ARRAY_H_

#include <unsupported/Eigen/CXX11/Tensor>

// #pragma GCC diagnostic push
// // turn off the specific warning
// #pragma GCC diagnostic ignored "-Wignored-attributes"
// #include <unsupported/Eigen/CXX11/Tensor>
// // turn the warnings back on
// #pragma GCC diagnostic pop

namespace io::numpy
{

/*!
 * Uses eigen Tensor as numpy array for numeric types
 * @tparam T numeric type
 * @tparam Dims number of matrix dimensions
 */
template <typename T, int Dims>
struct ArrayImpl
{
    /*!
     * Set row major order and size_t as index type
     */
    using Type = typename Eigen::Tensor<T, Dims, Eigen::RowMajor>;
};

/*!
 * uses a custom class definition for array of strings
 * @tparam Dims number of matrix dimensions
 */
// template<int Dims>
// struct ArrayImpl<std::string, Dims>
// {
//     using Type = StrArray<Dims>;
// };

/*!
 * Deduces the matrix type for the input T type
 */
template <typename T, int Dims>
using Array = typename ArrayImpl<T, Dims>::Type;

/*!
 * index type for numpy array
 */
using Index = Eigen::Index;

}  // namespace io::numpy

#endif  // __IO_NUMPY_ARRAY_H_
