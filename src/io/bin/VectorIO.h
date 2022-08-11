
#ifndef BIN_VECTORIO_H
#define BIN_VECTORIO_H

#include <vector>
#include <fstream>


namespace bin
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
    template<typename T>
    static bool serializeVector(const std::vector<T> &data, const std::string filePath)
    {
        std::ofstream file(filePath, std::ios::binary);
        file.write((char *) &data[0], data.size() * sizeof(T));
        file.close();

        return true;
    }

    /*!
     * \brief deserialize binary data into a vector
     * \param filePath file that contains binary data
     * \return deserialized vector
     */
    template<typename T>
    static std::vector<T> deserializeVector(const std::string filePath)
    {
        std::ifstream file(filePath, std::ios::binary);
        file.seekg(0, std::ios::end); //go to end of file
        long fileSize = file.tellg();
        file.seekg(0, std::ios::beg); //go to begin of file

        std::vector<T> data(static_cast<size_t>(fileSize / sizeof(T)));
        file.read((char *) data.data(), fileSize);

        return data;
    }

private:
    VectorIO();
};

} // namespace bin

#endif // BIN_VECTORIO_H
