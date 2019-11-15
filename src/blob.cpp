#include "../include/gwasm/blob.hpp"

namespace gwasm {

Blob::Blob(std::filesystem::path&& path)
    : m_path{std::move(path)}
{}

std::ifstream
Blob::open() const
{
    return {m_path};
}

Output::Output(std::filesystem::path&& path)
    : m_path{std::move(path)}
{}

std::ofstream
Output::open() const
{
    return {m_path};
}

Blob
Output::to_blob() &&
{
    return Blob{std::move(m_path)};
}

} // namespace gwasm
