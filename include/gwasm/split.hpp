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

#ifndef GWASM_SPLIT_HPP
#define GWASM_SPLIT_HPP

#include <filesystem>

#include "blob.hpp"

namespace gwasm {

// this class allocates files for you
class SplitContext
{
private:
    std::filesystem::path m_work_dir;
    int m_id;

public:
    explicit SplitContext(std::filesystem::path work_dir);

    SplitContext(const SplitContext&) = delete;
    SplitContext& operator=(const SplitContext&) = delete;

    Output new_blob();
};

} // namespace gwasm

#endif // GWASM_SPLIT_HPP
