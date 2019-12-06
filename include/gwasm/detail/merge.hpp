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

#ifndef GWASM_DETAIL_MERGE_HPP
#define GWASM_DETAIL_MERGE_HPP

#include <fstream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "../error.hpp"
#include "json.hpp"
#include "step_args.hpp"
#include "task_def.hpp"
#include "utils.hpp"

namespace gwasm::detail {

using json = nlohmann::json;

// Merge = Callable<void(
//     int, char**,
//     std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>&&)>
template <typename SplitResultTuple,
          typename ExecuteResultTuple,
          typename Merge>
void
merge_step(Merge merge, const MergeStepArgs& args)
{
    static_assert(is_like_tuple_v<SplitResultTuple>);
    static_assert(is_like_tuple_v<ExecuteResultTuple>);
    static_assert(
        std::is_invocable_v<
            Merge,
            int,
            char**,
            std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>&&>);

    auto tasks = std::vector<std::vector<TaskArg>>(read_json(args.tasks_path));
    auto tasks_out =
        std::vector<std::vector<TaskArg>>(read_json(args.tasks_out_path));

    if (tasks.size() != tasks_out.size()) {
        throw GwasmError{"unequal size of tasks and tasks_out"};
    }

    auto results =
        std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>{};
    results.reserve(tasks.size());
    {
        auto tasks_it = tasks.begin();
        const auto tasks_last = tasks.end();
        auto tasks_out_it = tasks_out.begin();
        for (; tasks_it != tasks_last; ++tasks_it, ++tasks_out_it) {
            results.push_back({vector_of_args_to_tuple<SplitResultTuple>(
                                   std::move(*tasks_it)),
                               vector_of_args_to_tuple<ExecuteResultTuple>(
                                   std::move(*tasks_out_it))});
        }
    }
    merge(args.argc, args.argv, std::move(results));
}

} // namespace gwasm::detail

#endif // GWASM_DETAIL_MERGE_HPP
