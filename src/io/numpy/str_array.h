//
// Created by Diego Aguirre on 3/12/20.
//

#ifndef __IO_NUMPY_STR_ARRAY_H_
#define __IO_NUMPY_STR_ARRAY_H_

#include <Eigen/Dense>
#include <cstring>
#include <locale>
#include <vector>
// #include <boost/locale/encoding_utf.hpp>

#include "reader.h"

namespace numpy
{

// using boost::locale::conv::utf_to_utf;

/*!
 * class for storing a numpy array of strings
 *
 * NOTE: numpy strings are in WIDE STRING (wstring) format
 * where each character has a byte size of 4 bytes.
 * Wstrings are stored linearly and in row major order. Each wstring
 * is a word and it has a fixed size "maxWordSize".
 *
 * When accessing data the word is returned in string format
 *
 * TODO: return a wchar when accessing data
 *
 * @tparam Dims numer of dimensions of the string matrix
 */
template <size_t Dims>
class StrArray
{
public:
    using Index = Eigen::Index;
    using Scalar = std::string;
    static constexpr size_t NumIndices = Dims;
    using Dimensions = std::array<Index, Dims>;

    StrArray() = default;

    StrArray(const Shape &shape, size_t maxWordSize, const Buffer &data)
        : maxWordSize(maxWordSize)
    {
        std::copy(shape.begin(), shape.end(), dimensions.begin());

        numWords = 1;
        for (const auto i : shape)
        {
            numWords *= i;
        }

        for (size_t i = 0; i < shape.size(); ++i)
        {
            size_t stride = maxWordSize;
            for (size_t j = i + 1; j < shape.size(); ++j)
            {
                stride *= shape[j];
            }

            strides.push_back(stride);
        }

        data.resize(numWords * maxWordSize);
        std::memcpy(&data[0], &data[0], data.size());
    }

    /*!
     * access data by indexes
     * converts indexes to a linear idx to bring the data
     * finally converts the recovered word to string format
     * @tparam Indexes indexes to recover the word. Number of index must match
     * the dimensions
     * @param firstIdx this arg is for setting a minimun number of indexes to 1
     * @param otherIndices the rest of the indexes needed to recover the word
     * @return recovered word as a string. Throws an exception if there is
     * an out of range error.
     */
    template <typename... Indexes>
    std::string operator()(Index firstIdx, Indexes... otherIndices)
    {
        std::array<Index, Dims> indexes{{firstIdx, otherIndices...}};
        if (indexes.size() != dimensions.size())
        {
            throw std::runtime_error("Wrong number of indexes");
        }

        for (Index i = 0; i < static_cast<Index>(dimensions.size()); ++i)
        {
            if (indexes[i] >= dimensions[i])
            {
                throw std::runtime_error("Out of range error!");
            }
        }

        size_t linearIdx = 0;
        for (size_t i = 0; i < indexes.size(); ++i)
        {
            linearIdx += strides[i] * indexes[i];
        }

        std::wstring wstring(&data[linearIdx], maxWordSize);
        std::string str = "";
        // std::string str = utf_to_utf<char>(wstring.c_str(), wstring.c_str() +
        // wstring.size());
        str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
        return str;
    }

    /*!
     * @return  number of dimensions of the array
     */
    const Dimensions &dimensions() { return dimensions; }

    /*!
     * @return data vector
     */
    const std::vector<wchar_t> &data() { return data; }

    /*!
     * @return number of words contained by the array
     */
    size_t getNumWords() { return numWords; }

    /*!
     * @return max word size
     */
    size_t getMaxWordSize() { return maxWordSize; }

private:
    std::vector<wchar_t> data;
    Dimensions dimensions;
    std::vector<size_t> strides;
    size_t numWords;
    size_t maxWordSize;
};

}  // namespace numpy

#endif  // __IO_NUMPY_STR_ARRAY_H_
