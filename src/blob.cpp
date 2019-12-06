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
