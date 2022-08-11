#include "Reader.h"
#include "../bin/VectorIO.h"

#include <sstream>
#include <iostream>
#include <filesystem>

namespace numpy
{

namespace fs = std::filesystem;

std::array<unsigned char, Reader::MAGIC_LEN> Reader::NUMPY_MAGIC = {0x93, 0x4E, 0x55, 0x4D, 0x50, 0x59};

const std::map<std::string, Endianness> Reader::NUMPY_ENDIANNESS_STR_MAP = {
        {"<", Endianness::LE},
        {"|", Endianness::LE},
        {">", Endianness::BE}
};

const std::string Reader::NUMPY_DATA_TYPE_SUBSTR = "'descr': '";

Reader::Reader()
{

}

File::Ptr Reader::readFile(const std::string &filePath)
{
    Buffer numpyBuffer = bin::VectorIO::deserializeVector<Byte>(filePath);
    return readNumpyBuffer(numpyBuffer);
}

File::Ptr Reader::readNumpyBuffer(const Buffer& numpyBuffer)
{
    File::Ptr numpyFilePtr;
    if(isNumpyFile(numpyBuffer))
    {
        try
        {
            numpyFilePtr = File::Ptr(new File);
            numpyFilePtr->_majorVersion = numpyBuffer[MAJOR_VERSION_POS];
            numpyFilePtr->_minorVersion = numpyBuffer[MINOR_VERSION_POS];
            size_t headerBytes = 0;
            std::tie(numpyFilePtr->_header, headerBytes) = getHeader(numpyBuffer);
            int arrayDataPos = HEADER_POS + headerBytes;
            numpyFilePtr->_arrayData = Buffer{numpyBuffer.begin() + arrayDataPos, numpyBuffer.end()};
        }
        catch(std::exception& e)
        {
            std::cerr << "Exception thrown while deserializing header with message: " << std::endl
                      << e.what() << std::endl;

            numpyFilePtr = nullptr;
        }
    }
    else
    {
        std::cerr << "Input buffer is not a numpy file" << std::endl;
    }

    return numpyFilePtr;
}

bool Reader::isNumpyFile(const Buffer &fileData)
{
    bool isNumpyFile = true;
    for (size_t i = 0; i < MAGIC_LEN; ++i)
    {
        isNumpyFile &= fileData[MAGIC_POS + i] == NUMPY_MAGIC[i];
    }

    return isNumpyFile;
}

std::pair<Header, size_t> Reader::getHeader(const Buffer &fileData)
{
    int length = 0;
    int shiftBits = 0;
    //header len is in little endian
    for (size_t i = 0; i < HEADER_LEN_LEN; ++i, shiftBits += 8)
    {
        length += fileData[HEADER_LEN_POS - i] << shiftBits;
    }

    int headerPos = HEADER_LEN_POS + HEADER_LEN_LEN;
    std::string headerStr((char*)&fileData[headerPos], length);
    Header header;
    header.shape = getArrayShapeFromHeaderString(headerStr);
    std::tie(header.endianness, header.dType) = processDescrField(headerStr);

    //finally store string
    header.str = std::move(headerStr);

    return {header, length};
}


std::pair<Endianness, DType> Reader::processDescrField(const std::string& headerStr)
{
    Endianness endianness = Endianness::UNKNOWN;
    DType dType = {DataType::UNKNOWN, 0};
    size_t pos = headerStr.find(NUMPY_DATA_TYPE_SUBSTR);
    if (pos != std::string::npos)
    {
        size_t startField = pos + NUMPY_DATA_TYPE_SUBSTR.size();
        size_t endField = headerStr.find('\'', startField);
        std::string endiannessStr = headerStr.substr(startField, 1);
        if (NUMPY_ENDIANNESS_STR_MAP.find(endiannessStr) != NUMPY_ENDIANNESS_STR_MAP.end())
        {
            endianness = NUMPY_ENDIANNESS_STR_MAP.at(endiannessStr);
        }
        else
        {
            std::string errorMsg = "UNKNOWN numpy endianness string " + endiannessStr;
            throw std::runtime_error(errorMsg);
        }

        const char dataTypeStr = headerStr[startField + 1];

        for(const auto& dataTypePair : DATA_TYPE_TO_STR)
        {
            if(dataTypePair.second == dataTypeStr)
            {
                dType.type = dataTypePair.first;
                break;
            }
        }

        size_t startBytes = startField + 2;
        size_t bytesLength = endField - startBytes;
        std::string bytesStr = headerStr.substr(startBytes, bytesLength);
        std::stringstream sstream(bytesStr);
        sstream >> dType.bytes;

        if (dType.type == DataType::UNKNOWN || dType.bytes == 0)
        {
            std::ostringstream ostream;
            ostream << "UNKNOWN numpy _data type string " << dataTypeStr << bytesStr << std::endl;
            throw std::runtime_error(ostream.str());
        }
    }

    return {endianness, dType};
}

Shape Reader::getArrayShapeFromHeaderString(const std::string &headerString)
{
    Shape shapeValues;
    const std::string shapeStr = "shape";
    const char startStr = '(';
    const char endStr = ')';

    std::string tmp = headerString.substr(headerString.find(shapeStr) + shapeStr.size(), headerString.size() - 1);
    size_t startPos = tmp.find(startStr);
    size_t endPos = tmp.find(endStr);
    size_t length = endPos - startPos - 1;
    std::string shapeValuesStr = tmp.substr(startPos + 1, length);

    std::stringstream ss(shapeValuesStr);
    for (int i; ss >> i;)
    {
        shapeValues.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    if(shapeValues.size() == 1)
    {
        //set col dimension to 1
        shapeValues.push_back(1);
    }

    return shapeValues;
}


} // namespace numpy
