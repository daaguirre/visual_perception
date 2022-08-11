
#ifndef NUMPYARRAYDESERIALIZER_H
#define NUMPYARRAYDESERIALIZER_H

#include <iostream>
#include <string>
#include "Reader.h"
#include "Array.h"

namespace numpy
{

/*!
 * struct template for copying data from a numpy buffer to a numpy array
 * @tparam T data type
 * @tparam ArrayT array type
 */
template<typename T, typename ArrayT>
struct ArrayData
{
    static void copy(const File& numpyFile, ArrayT& array)
    {
        static constexpr size_t NUM_DIMENSIONS = ArrayT::NumIndices;
        const Header& header = numpyFile.getHeader();
        const Buffer& arrayData = numpyFile.getArrayData();

        //TODO: take into account _data endianness
        Eigen::array<Eigen::Index, NUM_DIMENSIONS> shape;
        std::copy(header.shape.begin(), header.shape.end(), shape.begin());
        array = ArrayT(shape);
        std::memcpy(array.data(), &arrayData[0], arrayData.size());
    }
};

/*!
 * Partial specialization for copying form a numpy buffer to a numpy array of strings
 * @tparam ArrayT
 */
template<typename ArrayT>
struct ArrayData<std::string, ArrayT>
{
    static void copy(const File& numpyFile, ArrayT& array)
    {
        const Header& header = numpyFile.getHeader();
        const Buffer& arrayData = numpyFile.getArrayData();

        //TODO: take into account data endianness
        array = ArrayT(header.shape, header.dType.bytes, arrayData);
    }
};

/*!
 * class for serializing a deserializing numpy arrays
 */
class ArraySerializer
{
    ArraySerializer() = default;

    /*!
     * checks if can deserialize input numpy file. Only checks if the array in the
     * numpy file is the same as the expected one
     * @tparam ArrayT expected array type
     * @param header header of the input numpy file
     * @param array output array in the expected format
     * @return true if deserilization can start, otherwise false
     */
    template<typename ArrayT>
    static bool canDeserialize(const Header& header, ArrayT& array)
    {
        using DataT = typename ArrayT::Scalar;
        if(header.dType.type == DataType::UNKNOWN || header.dType.bytes == 0)
        {
            std::cerr << "Data type is unknown." << std::endl;
            return false;
        }

        if(header.shape.size() != array.dimensions().size())
        {
            std::cerr << "Expected Number of dimensions does not match the dimensions of the numpy array." << std::endl;
            return false;
        }

        DType expectedDType = ArrayType<DataT>::value();
        if(!header.dType.match(expectedDType))
        {
            std::cerr << "ArraySerializer Error because of type mismatching" << std::endl
                      << "Expected type is " << DATA_TYPE_TO_STR.at(ArrayType<DataT>::value().type)
                      << ArrayType<DataT>::value().bytes << std::endl
                      << "File type is " << DATA_TYPE_TO_STR.at(header.dType.type) << header.dType.bytes << std::endl;
            return false;
        }

        return true;
    }

public:

    /*!
     * Deserializes numpy file in path
     * @tparam ArrayT expected array type
     * @param numpyFilePath path where numpy file can be found
     * @param array output array
     * @return true if operation was successful, false otherwise
     */
    template<typename ArrayT>
    static bool deserialize(const std::string& numpyFilePath, ArrayT& array)
    {
        Reader reader;
        File::Ptr fileData = reader.readFile(numpyFilePath);
        if(fileData)
        {
            return deserialize(*fileData, array);
        }

        return false;
    }

    /*!
     * deserializes input numpy file
     * @tparam ArrayT expected array type
     * @param numpyFile input numpy file
     * @param array output array
     * @return true if operation was successful, false otherwise
     */
    template<typename ArrayT>
    static bool deserialize(const File& numpyFile, ArrayT& array)
    {
        if(!canDeserialize(numpyFile.getHeader(), array))
        {
            return false;
        }

        using DataT = typename ArrayT::Scalar;

        try
        {
            ArrayData<DataT, ArrayT>::copy(numpyFile, array);
        }
        catch(std::exception& e)
        {
            std::cerr << "Error deserilizing numpy array data with message: " << std::endl
                      << e.what() << std::endl;
            return false;
        }

        return true;
    }

    /*!
     * serializes input array to output path
     * @tparam ArrayT array type of the input array
     * @param path output path
     * @param array input array
     * @return true if operation was successful, false otherwise
     */
    template<typename ArrayT>
    static bool serialize(const std::string path, const ArrayT& array)
    {
        throw std::runtime_error("Not implemented error");
    }

};

}

#endif // NUMPYARRAYDESERIALIZER_H
