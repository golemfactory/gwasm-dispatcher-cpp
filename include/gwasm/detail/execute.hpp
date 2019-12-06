#ifndef GWASM_DETAIL_EXECUTE_HPP
#define GWASM_DETAIL_EXECUTE_HPP

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

// Execute == Callable<ExecuteResultTuple>(const SplitResultTupleArgs&...)>
template <typename SplitResultTuple, typename Execute>
void
execute_step(Execute execute, const ExecuteStepArgs& args)
{
    static_assert(is_like_tuple_v<SplitResultTuple>);
    static_assert(is_applicable_v<Execute, SplitResultTuple>);
    using ExecuteResultTuple = apply_result_t<Execute, SplitResultTuple>;
    static_assert(is_like_tuple_v<ExecuteResultTuple>);

    const auto json_task_to_args_tuple = [](json&& json_task) {
        return vector_of_args_to_tuple<SplitResultTuple>(
            std::vector<TaskArg>(std::move(json_task)));
    };

    auto result =
        std::apply(execute, json_task_to_args_tuple(read_json(args.task_path)));

    auto json_task_out = json::array();
    for_each_in_tuple(std::move(result), [&](auto&& i) {
        json_task_out.push_back(to_arg(std::move(i), {}));
    });
    std::ofstream{args.task_out_path} << json_task_out;
}

} // namespace gwasm::detail

#endif // GWASM_DETAIL_EXECUTE_HPP
