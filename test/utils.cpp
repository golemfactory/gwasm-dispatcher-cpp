#include "utils.hpp"

std::string
read_file_contents(std::ifstream&& file)
{
    file.seekg(0, std::ios::end);
    const auto size = file.tellg();
    auto buffer = std::string(size, '\0');
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}
