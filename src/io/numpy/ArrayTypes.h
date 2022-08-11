//
// Created by Diego Aguirre on 18/11/20.
//


#ifndef ARRAYTYPES_H
#define ARRAYTYPES_H

#include <stdexcept>
#include <exception>

namespace numpy
{

/*!
 * Numpy Data types
 */
enum class DataType
{
    U,
    I,
    F,
    UNICODE_STR,
    UNKNOWN,
};

/*!
 * A numpy dtype is defined by _data type and its size in bytes
 */
struct DType
{
    DataType type;
    size_t bytes;

    /*!
     * checks if input DType matches current type
     * @param other input dtype
     * @return true if DTypes are compatible, otherwise false
     */
    bool match(const DType& other) const
    {
        if(type != other.type)
        {
            return false;
        }

        //for type string the bytes field can be different
        if(type != DataType::UNICODE_STR)
        {
            return bytes == other.bytes;
        }

        return true;
    }
};

/*!
 * Numpy _data type strings
 */
const std::map<DataType, char> DATA_TYPE_TO_STR = {
        {DataType::U, 'u'},
        {DataType::I, 'i'},
        {DataType::F, 'f'},
        {DataType::UNICODE_STR, 'U'},
};

/*!
 * Each numpy _data type must relate to a C++ type in order to
 * be possible the decodification and codification of the numpy _data
 * @tparam T C++ type
 */
template<typename T>
struct ArrayType
{
    /*!
     * @return the corresponding Numpy DataType for the T type
     */
    static DType value()
    {
        throw std::runtime_error("Data type not supported");
    }
};

template<>
struct ArrayType<unsigned char>
{
    static DType value()
    {
        return {DataType::U, 1};
    }
};

template<>
struct ArrayType<char>
{
    static DType value()
    {
        return {DataType::I, 1};
    }
};

template<>
struct ArrayType<unsigned short>
{
    static DType value()
    {
        return {DataType::U, 2};
    }
};

template<>
struct ArrayType<short>
{
    static DType value()
    {
        return {DataType::I, 2};
    }
};

template<>
struct ArrayType<unsigned int>
{
    static DType value()
    {
        return {DataType::U, 4};
    }
};

template<>
struct ArrayType<int>
{
    static DType value()
    {
        return {DataType::I, 4};
    }
};

template<>
struct ArrayType<float>
{
    static DType value()
    {
        return {DataType::F, 4};
    }
};

template<>
struct ArrayType<unsigned long>
{
    static DType value()
    {
        return {DataType::U, 8};
    }
};

template<>
struct ArrayType<long>
{
    static DType value()
    {
        return {DataType::I, 8};
    }
};

template<>
struct ArrayType<double>
{
    static DType value()
    {
        return {DataType::F, 8};
    }
};

template<>
struct ArrayType<std::string>
{
    static DType value()
    {
        return {DataType::UNICODE_STR, std::numeric_limits<size_t>::max()};
    }
};

template<>
struct ArrayType<std::wstring>
{
    static DType value()
    {
        return {DataType::UNICODE_STR, std::numeric_limits<size_t>::max()};
    }
};

}

#endif //HIGHLEVELENTITIES_ARRAYTYPES_H
