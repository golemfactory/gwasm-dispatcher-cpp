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
//             const Args&,
//             std::vector<std::pair<DescTuple, ResultTuple>>&&)>
template <typename Merge, typename DescTuple, typename ResultTuple>
void
merge_step(Merge&& merge, const MergeStepArgs& args)
{
    static_assert(is_like_tuple_v<DescTuple>);
    static_assert(is_like_tuple_v<ResultTuple>);
    static_assert(
        std::is_invocable_v<Merge,
                            const Args&,
                            std::vector<std::pair<DescTuple, ResultTuple>>&&>);

    auto tasks = std::vector<std::vector<TaskArg>>(read_json(args.tasks_path));
    auto tasks_out =
        std::vector<std::vector<TaskArg>>(read_json(args.tasks_out_path));

    if (tasks.size() != tasks_out.size()) {
        throw GwasmError{"unequal size of tasks and tasks_out"};
    }

    auto results = std::vector<std::pair<DescTuple, ResultTuple>>{};
    results.reserve(tasks.size());
    {
        auto tasks_it = tasks.begin();
        const auto tasks_last = tasks.end();
        auto tasks_out_it = tasks_out.begin();
        for (; tasks_it != tasks_last; ++tasks_it, ++tasks_out_it) {
            results.push_back(
                {vector_of_args_to_tuple<DescTuple>(std::move(*tasks_it)),
                 vector_of_args_to_tuple<ResultTuple>(
                     std::move(*tasks_out_it))});
        }
    }
    merge(args.args, std::move(results));
}

} // namespace gwasm::detail

#endif // GWASM_DETAIL_MERGE_HPP
