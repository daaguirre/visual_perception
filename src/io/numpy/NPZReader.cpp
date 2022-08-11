//
// Created by diego on 12/11/20.
//

#include "NPZReader.h"
#include <zlib.h>
#include <exception>
#include <iostream>
#include <filesystem>


namespace numpy
{

namespace fs = std::filesystem;

const std::string NPZReader::NPZ_EXTENSION = ".npz";

NPZData NPZReader::readNPZFile(const std::string &filePath)
{
    NPZData npzData;
    bool success = checkExtension(filePath);
    if(!success)
    {
        std::cerr << "input path is not a npz file: " << filePath << std::endl;
        return npzData;
    }

    FILE *npzFile = fopen(filePath.c_str(), "rb");
    if (npzFile == NULL)
    {
        std::cerr << "NPZ file not found at path " << filePath << std::endl;
        return npzData;
    }

    try
    {
        npzData = parseZipFile(npzFile);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error while reading npz file with message " << e.what() << std::endl;
        std::cerr << "Returning empty npz data map" << std::endl;
    }

    fclose(npzFile);

    return npzData;
}

NPZData NPZReader::parseZipFile(FILE *npzFile)
{
    NPZData npzData;
    while (true)
    {
        std::vector<char> localHeaderArray(HEADER_SIZE);
        size_t headerRes = fread(&localHeaderArray[0], sizeof(char), HEADER_SIZE, npzFile);
        if (headerRes != HEADER_SIZE)
        {
            throw std::runtime_error("npz load: failed fread");
        }

        //if we've reached the global header, stop reading
        if (localHeaderArray[2] != 0x03 || localHeaderArray[3] != 0x04)
        {
            break;
        }

        ZipLocalHeader localHeader = parseZipLocalHeader(localHeaderArray, npzFile);
        if (localHeader.zipMethod != NO_COMPRESSED_METHOD)
        {
            File::Ptr numpyFilePtr = unzipNumpyFile(localHeader, npzFile);
            if (numpyFilePtr != nullptr)
            {
                //input data localHeader.arrayName and numpyFilePtr will be empty after this operation
                //remove move operation if needed later
                npzData.insert({std::move(localHeader.arrayName), std::move(*numpyFilePtr)});
            }
        }
        else
        {
            throw std::runtime_error("Deserialization of uncompressed npz files not implemented yet.");
        }
    }

    return npzData;
}

NPZReader::ZipLocalHeader NPZReader::parseZipLocalHeader(const std::vector<char> &localHeaderArray, FILE *npzFile)
{
    ZipLocalHeader localHeader;
    //read in the variable name
    uint16_t nameLen = *(uint16_t *) &localHeaderArray[26];
    char *name_c = new char[nameLen + 1];
    size_t varnameRes = fread(name_c, sizeof(char), nameLen, npzFile);
    if (varnameRes != nameLen)
    {
        throw std::runtime_error("parseZipLocalHeader: failed fread var name");
    }
    name_c[nameLen] = '\0';
    std::string varname(name_c);
    delete[] name_c;

    //erase the lagging .npy
    varname.erase(varname.end() - 4, varname.end());

    //read in the extra field
    uint16_t extraFieldLen = *(uint16_t *) &localHeaderArray[28];
    if (extraFieldLen > 0)
    {
        std::vector<char> buff(extraFieldLen);
        size_t extraFieldRes = fread(&buff[0], sizeof(char), extraFieldLen, npzFile);
        if (extraFieldRes != extraFieldLen)
        {
            throw std::runtime_error("npz load: failed fread on extra field");
        }
    }

    localHeader.zipMethod = *reinterpret_cast<const uint16_t *>(&localHeaderArray[0] + 8);
    localHeader.zippedBytes = *reinterpret_cast<const uint32_t *>(&localHeaderArray[0] + 18);
    localHeader.unzippedBytes = *reinterpret_cast<const uint32_t *>(&localHeaderArray[0] + 22);
    localHeader.arrayName = std::move(varname);

    return localHeader;
}

File::Ptr NPZReader::unzipNumpyFile(const NPZReader::ZipLocalHeader &localHeader, FILE *npzFile)
{
    std::vector<unsigned char> zippedData(localHeader.zippedBytes);
    std::vector<unsigned char> unzippedData(localHeader.unzippedBytes);
    size_t nread = fread(&zippedData[0], 1, localHeader.zippedBytes, npzFile);
    if (nread != localHeader.zippedBytes)
    {
        throw std::runtime_error("unzipNumpyFile: failed fread zipped bytes");
    }

    int ret;
    z_stream zipStream = {0};

    zipStream.zalloc = Z_NULL;
    zipStream.zfree = Z_NULL;
    zipStream.opaque = Z_NULL;

    zipStream.total_in  = zipStream.avail_in  = localHeader.zippedBytes;
    zipStream.total_out = zipStream.avail_out = localHeader.unzippedBytes;
    zipStream.next_in   = (Bytef *) &zippedData[0];
    zipStream.next_out  = (Bytef *) &unzippedData[0];

    ret = inflateInit2(&zipStream, -MAX_WBITS);
    if (ret != Z_OK)
    {
        inflateEnd(&zipStream);
        throw std::runtime_error("unzipNumpyFile: Error initializing inflate process.");
    }
    ret = inflate(&zipStream, Z_FINISH);
    inflateEnd(&zipStream);
    if(ret != Z_STREAM_END)
    {
        throw std::runtime_error("unzipNumpyFile: Error during inflate process.");
    }

    File::Ptr numpyFilePtr = _numpyReader.readNumpyBuffer(unzippedData);
    return numpyFilePtr;
}

bool NPZReader::checkExtension(const std::string& filePath)
{
    std::string extension = fs::path(filePath).extension();

    return extension == NPZ_EXTENSION;
}


}