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

#include <sstream>
#include <string_view>

#include "../../include/gwasm/detail/step_args.hpp"
#include "../../include/gwasm/detail/utils.hpp"

namespace gwasm::detail {

namespace {

std::variant<SplitStepArgs, Error>
parse_split_args(int argc, char* argv[])
{
    if (argc < 1) {
        return Error{"missing work_dir"};
    }
    return SplitStepArgs{
        .argc = argc - 1,
        .argv = argv + 1,
        .work_dir = argv[0],
    };
}

std::variant<ExecuteStepArgs, Error>
parse_execute_args(int argc, char* argv[])
{
    if (argc != 2) {
        return Error{"expected exactly 2 arguments"};
    }
    return ExecuteStepArgs{
        .task_path = argv[0],
        .task_out_path = argv[1],
    };
}

std::variant<MergeStepArgs, Error>
parse_merge_args(int argc, char* argv[])
{
    if (argc < 3) {
        return Error{"expected at least 3 arguments"};
    }
    if (std::string_view{argv[2]} != "--") {
        return Error{"expected \"--\" as third argument"};
    }
    return MergeStepArgs{
        .argc = argc - 3,
        .argv = argv + 3,
        .tasks_path = argv[0],
        .tasks_out_path = argv[1],
    };
}

using ReturnT =
    std::variant<SplitStepArgs, ExecuteStepArgs, MergeStepArgs, Error>;

} // namespace

ReturnT
parse_args(int argc, char* argv[])
{
    if (argc < 2) {
        return Error{"Missing split, merge or execute command"};
    }

    const auto command = std::string_view{argv[1]};

    if (command == "split") {
        return extend_variant<ReturnT>(parse_split_args(argc - 2, argv + 2));
    }
    else if (command == "exec") {
        return extend_variant<ReturnT>(parse_execute_args(argc - 2, argv + 2));
    }
    else if (command == "merge") {
        return extend_variant<ReturnT>(parse_merge_args(argc - 2, argv + 2));
    }

    auto msg = std::ostringstream{};
    msg << "unknown command \"" << command << '"';
    return Error{msg.str()};
}

} // namespace gwasm::detail
