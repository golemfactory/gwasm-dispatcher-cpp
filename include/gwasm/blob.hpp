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

#ifndef GWASM_BLOB_HPP
#define GWASM_BLOB_HPP

#include <filesystem>
#include <fstream>
#include <utility>
#include <variant>

namespace gwasm {

class Blob;
class Output;

namespace detail {

struct TaskArgMeta;
struct TaskArgBlob;
struct TaskArgOutput;
using TaskArg = std::variant<TaskArgMeta, TaskArgBlob, TaskArgOutput>;

TaskArg
to_arg(const Blob&, const std::filesystem::path&);
TaskArg
to_arg(const Output&, const std::filesystem::path&);

struct ExecuteStepArgs;

template <typename SplitResultTuple, typename Execute>
void
execute_step(Execute execute, const detail::ExecuteStepArgs& args);

} // namespace detail

// read-only file access
class Blob
{
private:
    friend detail::TaskArg detail::to_arg(const Blob&,
                                          const std::filesystem::path&);

    std::filesystem::path m_absolute_path;

public:
    Blob() = default;
    explicit Blob(std::filesystem::path&& absolute_path);

    std::ifstream open() const;
};

// read-write file access
class Output
{
private:
    friend detail::TaskArg detail::to_arg(const Output&,
                                          const std::filesystem::path&);
    template <typename SplitResultTuple, typename Execute>
    friend void detail::execute_step(Execute execute,
                                     const detail::ExecuteStepArgs& args);

    std::filesystem::path m_absolute_path;

public:
    Output() = default;
    explicit Output(std::filesystem::path&& absolute_path);

    std::ofstream open() const;

    Blob to_blob() &&;
};

} // namespace gwasm

#endif // GWASM_BLOB_HPP
