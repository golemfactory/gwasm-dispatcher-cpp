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

#ifndef GWASM_DETAIL_STEP_ARGS_HPP
#define GWASM_DETAIL_STEP_ARGS_HPP

#include <filesystem>
#include <string>
#include <variant>

namespace gwasm::detail {

struct SplitStepArgs
{
    int argc;
    char** argv;
    std::filesystem::path work_dir;
};

struct ExecuteStepArgs
{
    std::filesystem::path task_path;
    std::filesystem::path task_out_path;
};

struct MergeStepArgs
{
    int argc;
    char** argv;
    std::filesystem::path tasks_path;
    std::filesystem::path tasks_out_path;
};

struct Error
{
    std::string message;
    int exit_code = 1;
};

std::variant<SplitStepArgs, ExecuteStepArgs, MergeStepArgs, Error>
parse_args(int argc, char* argv[]);

} // namespace gwasm::detail

#endif // GWASM_DETAIL_ARGS_HPP
