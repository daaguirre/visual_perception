
#include "reader.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#include "io/bin/vector_io.h"

namespace io::numpy
{

namespace fs = std::filesystem;

std::array<unsigned char, Reader::MAGIC_LEN> Reader::NUMPY_MAGIC = {0x93, 0x4E, 0x55,
                                                                    0x4D, 0x50, 0x59};

const std::map<std::string, Endianness> Reader::NUMPY_ENDIANNESS_STR_MAP = {
    {"<", Endianness::LE}, {"|", Endianness::LE}, {">", Endianness::BE}
};

const std::string Reader::NUMPY_DATA_TYPE_SUBSTR = "'descr': '";

Reader::Reader() {}

File::Ptr Reader::readFile(const std::string& file_path)
{
    Buffer numpy_buffer = bin::VectorIO::deserializeVector<Byte>(file_path);
    return readNumpyBuffer(numpy_buffer);
}

File::Ptr Reader::readNumpyBuffer(const Buffer& numpy_buffer)
{
    File::Ptr numpy_file_ptr;
    if (isNumpyFile(numpy_buffer))
    {
        try
        {
            numpy_file_ptr = File::Ptr(new File);
            numpy_file_ptr->major_version = numpy_buffer[MAJOR_VERSION_POS];
            numpy_file_ptr->minor_version = numpy_buffer[MINOR_VERSION_POS];
            size_t headerBytes = 0;
            std::tie(numpy_file_ptr->header, headerBytes) = getHeader(numpy_buffer);
            int array_data_pos = HEADER_POS + headerBytes;
            numpy_file_ptr->array_data =
                Buffer{numpy_buffer.begin() + array_data_pos, numpy_buffer.end()};
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception thrown while deserializing header with message: " << std::endl
                      << e.what() << std::endl;

            numpy_file_ptr = nullptr;
        }
    }
    else
    {
        std::cerr << "Input buffer is not a numpy file" << std::endl;
    }

    return numpy_file_ptr;
}

bool Reader::isNumpyFile(const Buffer& file_data)
{
    bool is_numpy_file = true;
    for (size_t i = 0; i < MAGIC_LEN; ++i)
    {
        is_numpy_file &= file_data[MAGIC_POS + i] == NUMPY_MAGIC[i];
    }

    return is_numpy_file;
}

std::pair<Header, size_t> Reader::getHeader(const Buffer& file_data)
{
    int length = 0;
    int shift_bits = 0;
    // header len is in little endian
    for (size_t i = 0; i < HEADER_LEN_LEN; ++i, shift_bits += 8)
    {
        length += file_data[HEADER_LEN_POS - i] << shift_bits;
    }

    int header_pos = HEADER_LEN_POS + HEADER_LEN_LEN;
    std::string header_str((char*)&file_data[header_pos], length);
    Header header;
    header.shape = getArrayShapeFromHeaderString(header_str);
    std::tie(header.endianness, header.dtype) = processDescrField(header_str);

    // finally store string
    header.str = std::move(header_str);

    return {header, length};
}

std::pair<Endianness, DType> Reader::processDescrField(const std::string& header_str)
{
    Endianness endianness = Endianness::UNKNOWN;
    DType dtype = {DataType::UNKNOWN, 0};
    size_t pos = header_str.find(NUMPY_DATA_TYPE_SUBSTR);
    if (pos != std::string::npos)
    {
        size_t start_field = pos + NUMPY_DATA_TYPE_SUBSTR.size();
        size_t end_field = header_str.find('\'', start_field);
        std::string endianness_str = header_str.substr(start_field, 1);
        if (NUMPY_ENDIANNESS_STR_MAP.find(endianness_str) != NUMPY_ENDIANNESS_STR_MAP.end())
        {
            endianness = NUMPY_ENDIANNESS_STR_MAP.at(endianness_str);
        }
        else
        {
            std::string errorMsg = "UNKNOWN numpy endianness string " + endianness_str;
            throw std::runtime_error(errorMsg);
        }

        const char data_type_str = header_str[start_field + 1];

        for (const auto& data_type_pair : DATA_TYPE_TO_STR)
        {
            if (data_type_pair.second == data_type_str)
            {
                dtype.type = data_type_pair.first;
                break;
            }
        }

        size_t start_bytes = start_field + 2;
        size_t bytes_length = end_field - start_bytes;
        std::string bytes_str = header_str.substr(start_bytes, bytes_length);
        std::stringstream sstream(bytes_str);
        sstream >> dtype.bytes;

        if (dtype.type == DataType::UNKNOWN || dtype.bytes == 0)
        {
            std::ostringstream ostream;
            ostream << "UNKNOWN numpy _data type string " << data_type_str << bytes_str
                    << std::endl;
            throw std::runtime_error(ostream.str());
        }
    }

    return {endianness, dtype};
}

Shape Reader::getArrayShapeFromHeaderString(const std::string& header_string)
{
    Shape shape_values;
    const std::string shape_str = "shape";
    const char start_str = '(';
    const char end_str = ')';

    std::string tmp = header_string.substr(header_string.find(shape_str) + shape_str.size(),
                                           header_string.size() - 1);
    size_t start_pos = tmp.find(start_str);
    size_t end_pos = tmp.find(end_str);
    size_t length = end_pos - start_pos - 1;
    std::string shape_values_str = tmp.substr(start_pos + 1, length);

    std::stringstream ss(shape_values_str);
    for (int i; ss >> i;)
    {
        shape_values.push_back(i);
        if (ss.peek() == ',') ss.ignore();
    }

    if (shape_values.size() == 1)
    {
        // set col dimension to 1
        shape_values.push_back(1);
    }

    return shape_values;
}

}  // namespace io::numpy
