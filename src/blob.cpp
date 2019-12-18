/*
Copyright 2019 Golem Factory GmbH

This file is part of gwasm dispatcher cpp.

gwasm dispatcher cpp is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gwasm dispatcher cpp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with gwasm dispatcher cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "../include/gwasm/blob.hpp"

#include <system_error>

namespace gwasm {

Blob::Blob(std::filesystem::path&& absolute_path)
    : m_absolute_path{std::move(absolute_path)}
{}

std::ifstream
Blob::open() const
{
    if (!std::filesystem::is_regular_file(m_absolute_path)) {
        throw std::filesystem::filesystem_error{
            "file not found",
            m_absolute_path,
            std::make_error_code(std::errc::no_such_file_or_directory)};
    }
    return std::ifstream{m_absolute_path};
}

Output::Output(std::filesystem::path&& absolute_path)
    : m_absolute_path{std::move(absolute_path)}
{}

std::ofstream
Output::open() const
{
    return std::ofstream{m_absolute_path};
}

Blob
Output::to_blob() &&
{
    return Blob{std::move(m_absolute_path)};
}

} // namespace gwasm
