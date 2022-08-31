//
// Created by diego on 12/11/20.
//

#include "npz_reader.h"

#include <zlib.h>

#include <exception>
#include <filesystem>
#include <iostream>

namespace io::numpy
{

namespace fs = std::filesystem;

const std::string NPZReader::NPZ_EXTENSION = ".npz";

NPZData NPZReader::read_npz_file(const std::string &file_path)
{
    NPZData npz_data;
    bool success = check_extension(file_path);
    if (!success)
    {
        std::cerr << "input path is not a npz file: " << file_path << std::endl;
        return npz_data;
    }

    FILE *npz_file = fopen(file_path.c_str(), "rb");
    if (npz_file == NULL)
    {
        std::cerr << "NPZ file not found at path " << file_path << std::endl;
        return npz_data;
    }

    try
    {
        npz_data = parse_zip_file(npz_file);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error while reading npz file with message " << e.what() << std::endl;
        std::cerr << "Returning empty npz data map" << std::endl;
    }

    fclose(npz_file);

    return npz_data;
}

NPZData NPZReader::parse_zip_file(FILE *npz_file)
{
    NPZData npz_data;
    while (true)
    {
        std::vector<char> local_header_array(HEADER_SIZE);
        size_t headerRes = fread(&local_header_array[0], sizeof(char), HEADER_SIZE, npz_file);
        if (headerRes != HEADER_SIZE)
        {
            throw std::runtime_error("npz load: failed fread");
        }

        // if we've reached the global header, stop reading
        if (local_header_array[2] != 0x03 || local_header_array[3] != 0x04)
        {
            break;
        }

        ZipLocalHeader local_header = parse_zip_local_header(local_header_array, npz_file);
        if (local_header.zip_method != NO_COMPRESSED_METHOD)
        {
            File::Ptr numpy_file_ptr = unzip_numpy_file(local_header, npz_file);
            if (numpy_file_ptr != nullptr)
            {
                // input data local_header.array_name and numpy_file_ptr will be
                // empty after this operation remove move operation if needed
                // later
                npz_data.insert({std::move(local_header.array_name), std::move(*numpy_file_ptr)});
            }
        }
        else
        {
            throw std::runtime_error(
                "Deserialization of uncompressed npz files not implemented "
                "yet.");
        }
    }

    return npz_data;
}

NPZReader::ZipLocalHeader NPZReader::parse_zip_local_header(
    const std::vector<char> &local_header_array,
    FILE *npz_file)
{
    ZipLocalHeader localHeader;
    // read in the variable name
    uint16_t name_len = *(uint16_t *)&local_header_array[26];
    char *name_c = new char[name_len + 1];
    size_t var_name_res = fread(name_c, sizeof(char), name_len, npz_file);
    if (var_name_res != name_len)
    {
        throw std::runtime_error("parseZipLocalHeader: failed fread var name");
    }
    name_c[name_len] = '\0';
    std::string var_name(name_c);
    delete[] name_c;

    // erase the lagging .npy
    var_name.erase(var_name.end() - 4, var_name.end());

    // read in the extra field
    uint16_t extra_field_len = *(uint16_t *)&local_header_array[28];
    if (extra_field_len > 0)
    {
        std::vector<char> buff(extra_field_len);
        size_t extraFieldRes = fread(&buff[0], sizeof(char), extra_field_len, npz_file);
        if (extraFieldRes != extra_field_len)
        {
            throw std::runtime_error("npz load: failed fread on extra field");
        }
    }

    localHeader.zip_method = *reinterpret_cast<const uint16_t *>(&local_header_array[0] + 8);
    localHeader.zipped_bytes = *reinterpret_cast<const uint32_t *>(&local_header_array[0] + 18);
    localHeader.unzipped_bytes = *reinterpret_cast<const uint32_t *>(&local_header_array[0] + 22);
    localHeader.array_name = std::move(var_name);

    return localHeader;
}

File::Ptr NPZReader::unzip_numpy_file(const NPZReader::ZipLocalHeader &local_header, FILE *npz_file)
{
    std::vector<unsigned char> zippedData(local_header.zipped_bytes);
    std::vector<unsigned char> unzipped_data(local_header.unzipped_bytes);
    size_t nread = fread(&zippedData[0], 1, local_header.zipped_bytes, npz_file);
    if (nread != local_header.zipped_bytes)
    {
        throw std::runtime_error("unzipNumpyFile: failed fread zipped bytes");
    }

    int ret;
    z_stream zip_stream = {0};

    zip_stream.zalloc = Z_NULL;
    zip_stream.zfree = Z_NULL;
    zip_stream.opaque = Z_NULL;

    zip_stream.total_in = zip_stream.avail_in = local_header.zipped_bytes;
    zip_stream.total_out = zip_stream.avail_out = local_header.unzipped_bytes;
    zip_stream.next_in = (Bytef *)&zippedData[0];
    zip_stream.next_out = (Bytef *)&unzipped_data[0];

    ret = inflateInit2(&zip_stream, -MAX_WBITS);
    if (ret != Z_OK)
    {
        inflateEnd(&zip_stream);
        throw std::runtime_error("unzipNumpyFile: Error initializing inflate process.");
    }
    ret = inflate(&zip_stream, Z_FINISH);
    inflateEnd(&zip_stream);
    if (ret != Z_STREAM_END)
    {
        throw std::runtime_error("unzipNumpyFile: Error during inflate process.");
    }

    File::Ptr numpy_file_ptr = m_numpy_reader.read_numpy_buffer(unzipped_data);
    return numpy_file_ptr;
}

bool NPZReader::check_extension(const std::string &file_path)
{
    std::string extension = fs::path(file_path).extension();

    return extension == NPZ_EXTENSION;
}

}  // namespace io::numpy