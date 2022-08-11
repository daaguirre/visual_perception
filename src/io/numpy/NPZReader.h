//
// Created by Diego Aguirre on 12/11/20.
//

#ifndef HIGHLEVELENTITIES_NPZREADER_H
#define HIGHLEVELENTITIES_NPZREADER_H

#include "Reader.h"

namespace numpy
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
     * @param filePath input path
     * @return a non empty map with the unzipped files and their names if operation
     * was successful, otherwise empty
     */
    NPZData readNPZFile(const std::string& filePath);

private:
    /*!
     * fields of a local header in a zip file
     */
    struct ZipLocalHeader
    {
        std::string arrayName;
        uint16_t  zipMethod;
        unsigned int zippedBytes;
        unsigned int unzippedBytes;
    };

    Reader _numpyReader;

    /*!
     * parses input npz file as a regular zip file
     * @param npzFile input npz file
     * @return uncompressed numpy files
     */
    NPZData parseZipFile(FILE* npzFile);

    /*!
     * parses local header of a zip file
     * @param localHeaderArray input local header array
     * @param npzFile pointer to FILE to read extra data
     * @return The local header
     */
    ZipLocalHeader parseZipLocalHeader(const std::vector<char>& localHeaderArray, FILE* npzFile);

    /*!
     * unzips numpyfile
     * @param localHeader previously parsed local header
     * @param npzFile pointer to FILE to obtain zipped bytes
     * @return unzipped numpy file. If operation was successful is a valid file,
     * otherwise invalid
     */
    File::Ptr unzipNumpyFile(const ZipLocalHeader& localHeader, FILE* npzFile);

    /*!
     * checks if input file has correct extension
     * @param filePath input file path
     * @return true if extension is correct, otherwise false
     */
    static bool checkExtension(const std::string& filePath);

};

} // namespace numpy

#endif //HIGHLEVELENTITIES_NPZREADER_H
