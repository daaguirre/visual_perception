
#ifndef __IO_BIN_VECTOR_IO_H_
#define __IO_BIN_VECTOR_IO_H_

#include <fstream>
#include <vector>

namespace io::bin
{

class VectorIO
{
public:
    /*!
     * \brief serialize std::vector to a binary file
     * \param data std vector to serialize
     * \param filePath file where to write binary data
     * \return result
     */
    template <typename T>
    static bool serializeVector(const std::vector<T> &data, const std::string& file_path)
    {
        std::ofstream file(file_path, std::ios::binary);
        file.write((char *)&data[0], data.size() * sizeof(T));
        file.close();

        return true;
    }

    /*!
     * \brief deserialize binary data into a vector
     * \param file_path file that contains binary data
     * \return deserialized vector
     */
    template <typename T>
    static std::vector<T> deserializeVector(const std::string& file_path)
    {
        std::ifstream file(file_path, std::ios::binary);
        if (file.fail())
        {
            std::stringstream error_stream;
            error_stream << "Input file " << file_path << " not found\n";
            throw std::invalid_argument(error_stream.str());
        }

        file.seekg(0, std::ios::end);  // go to end of file
        long file_size = file.tellg();
        file.seekg(0, std::ios::beg);  // go to begin of file

        std::vector<T> data(static_cast<size_t>(file_size / sizeof(T)));
        file.read((char *)data.data(), file_size);

        return data;
    }

private:
    VectorIO();
};

}  // namespace bin

#endif // __IO_BIN_VECTOR_IO_H_
