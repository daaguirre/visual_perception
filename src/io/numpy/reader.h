
#ifndef __IO_NUMPY_READER_H_
#define __IO_NUMPY_READER_H_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "array_types.h"

namespace io::numpy
{

using Byte = unsigned char;
using Buffer = std::vector<Byte>;
using Shape = std::vector<int>;

class Reader;
class NPZReader;
class File;

/*!
 * Numpy endianness
 */
enum class Endianness
{
    LE,
    BE,
    UNKNOWN
};

/*!
 * Numpy file header
 */
struct Header
{
    std::string str;
    Shape shape;
    Endianness endianness;
    DType dtype;

    friend File;
    friend Reader;

private:
    Header() {}
};

/*!
 * This class represents a numpy file divided in 2 main parts:
 * HEADER: contains the header of the numpy file
 * NUMPY BUFFER: array data in byte format
 */
class File
{
public:
    using Ptr = std::shared_ptr<File>;
    using ConstPtr = std::shared_ptr<const File>;

    /*!
     * \brief gets numpy file Major Version
     * \return major version as unsigned char
     */
    unsigned char getMajorVersion() const { return major_version; }

    /*!
     * \brief get numpy file Minor Version
     * \return minor version as unsigned char
     */
    unsigned char getMinorVersion() const { return minor_version; }

    /*!
     * \brief gets numpy file header
     * \return
     */
    const Header &getHeader() const { return header; }

    /*!
     * \brief gets Array Data
     * \return as a vector of bytes
     */
    const Buffer &getArrayData() const { return array_data; }

    friend Reader;
    friend NPZReader;
    friend std::pair<const std::string, File>;

private:
    File() {}

    unsigned char major_version;
    unsigned char minor_version;
    Header header;
    Buffer array_data;
};

class Reader
{
    /*!
     * \brief lenght of numpy magic string in bytes
     */
    static constexpr unsigned char MAGIC_LEN = 6;
    /*!
     * \brief starting position of magic string in binary file
     */
    static constexpr unsigned char MAGIC_POS = 0;
    /*!
     * \brief position of major version in binary file
     */
    static constexpr unsigned char MAJOR_VERSION_POS = 6;
    /*!
     * \brief position of minor version in binary file
     */
    static constexpr unsigned char MINOR_VERSION_POS = 7;
    /*!
     * \brief position of HEADER_LEN section in binary file
     */
    static constexpr unsigned char HEADER_LEN_POS = 8;
    /*!
     * \brief len of HEADER_LEN section
     */
    static constexpr unsigned char HEADER_LEN_LEN = 2;

    /*!
     * \brief starting position of Header Data section
     */
    static constexpr unsigned char HEADER_POS = HEADER_LEN_POS + HEADER_LEN_LEN;

    /*!
     * \brief numpy magic string in binary format
     */
    static std::array<unsigned char, MAGIC_LEN> NUMPY_MAGIC;

    /*!
     * Endiannes string map
     */
    static const std::map<std::string, Endianness> NUMPY_ENDIANNESS_STR_MAP;

    /*!
     * \brief numpy data string identifier
     */
    static const std::string NUMPY_DATA_TYPE_SUBSTR;

public:
    Reader();

    /*!
     * \brief reads a numpy file
     * \param file_path path to file
     * \return numpy file deserialized as a File struct
     */
    File::Ptr readFile(const std::string &file_path);

    /*!
     * reads a numpy file from a binary buffer
     * @param buffer input binary buffer
     * @return a valid numpy file if operation was successful,
     * otherwise invalid
     */
    File::Ptr readNumpyBuffer(const Buffer &buffer);

protected:
    /*!
     * \brief looks for numpy magic string in file
     * \param file_data numpy file binary data
     * \return true if binary data represents a numpy file, false otherwise
     */
    bool isNumpyFile(const Buffer &file_data);

    /*!
     * \brief gets header from numpy file binary data
     * \param file_data numpy file binary data
     * \return numpy header as Header struct and the number of bytes
     * occupied by the header
     */
    std::pair<Header, size_t> getHeader(const Buffer &file_data);

    /*!
     * \brief gets array shape from header info
     * \param header_string numpy header string
     * \return array shape as a vector of ints
     */
    Shape getArrayShapeFromHeaderString(const std::string &header_string);

    /*!
     * processes the 'Descr' field of the numpy header
     * @param header_str input numpy header string
     * @return Endiannes and DataType
     */
    static std::pair<Endianness, DType> processDescrField(const std::string &header_str);
};

}  // namespace io::numpy

#endif  // __IO_NUMPY_READER_H_
