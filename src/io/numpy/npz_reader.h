//
// Created by Diego Aguirre on 12/11/20.
//

#ifndef __IO_NUMPY_NPZ_READER_H_
#define __IO_NUMPY_NPZ_READER_H_

#include "reader.h"

namespace io::numpy
{

/*!
 * map for storing the different numpy files found in
 * a npz file
 */
using NPZData = std::map<std::string, File>;

/*!
 * Class for reading a npz file
 */
class NPZReader
{
    static const std::string NPZ_EXTENSION;
    static constexpr size_t HEADER_SIZE = 30;
    static constexpr size_t NO_COMPRESSED_METHOD = 0;

public:
    NPZReader() = default;

    /*!
     * reads npz file from input path
     * @param file_path input path
     * @return a non empty map with the unzipped files and their names if
     * operation was successful, otherwise empty
     */
    NPZData read_npz_file(const std::string &filePath);

private:
    /*!
     * fields of a local header in a zip file
     */
    struct ZipLocalHeader
    {
        std::string array_name;
        uint16_t zip_method;
        unsigned int zipped_bytes;
        unsigned int unzipped_bytes;
    };

    Reader m_numpy_reader;

    /*!
     * parses input npz file as a regular zip file
     * @param npz_file input npz file
     * @return uncompressed numpy files
     */
    NPZData parse_zip_file(FILE *npz_file);

    /*!
     * parses local header of a zip file
     * @param local_header_array input local header array
     * @param npz_file pointer to FILE to read extra data
     * @return The local header
     */
    ZipLocalHeader parse_zip_local_header(const std::vector<char> &local_header_array, FILE *npz_file);

    /*!
     * unzips numpyfile
     * @param local_header previously parsed local header
     * @param npz_file pointer to FILE to obtain zipped bytes
     * @return unzipped numpy file. If operation was successful is a valid file,
     * otherwise invalid
     */
    File::Ptr unzip_numpy_file(const ZipLocalHeader &local_header, FILE *npz_file);

    /*!
     * checks if input file has correct extension
     * @param file_path input file path
     * @return true if extension is correct, otherwise false
     */
    static bool check_extension(const std::string &file_path);
};

}  // namespace io::numpy

#endif  // __IO_NUMPY_NPZ_READER_H_
