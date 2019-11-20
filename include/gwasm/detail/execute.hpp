#ifndef GWASM_DETAIL_EXECUTE_HPP
#define GWASM_DETAIL_EXECUTE_HPP

#include <fstream>
#include <tuple>
#include <type_traits>
#include <vector>

#include <nlohmann/json.hpp>

#include "args.hpp"
#include "task_def.hpp"
#include "utils.hpp"

namespace gwasm::detail {

using json = nlohmann::json;

template <typename... T>
std::tuple<T...>
vector_of_args_to_tuple(const std::vector<TaskArg>& vector)
{
    if (vector.size() != sizeof...(T)) {
        throw std::runtime_error{"wrong size"};
    }

    auto out = std::tuple<T...>{};
    for_each_in_tuple(
        [it = vector.begin()](auto& i) mutable { from_arg(i, *(it++), {}); },
        out);
    return out;
}

// Execute == Callable<Tuple<R...>(const Args&...)>
template <typename Execute, typename... Args>
void
execute_step(Execute&& execute, const ExecuteArgs& args)
{
    static_assert(std::is_invocable_v<Execute, const Args&...>);
    using ResultTuple = std::invoke_result_t<Execute, const Args&...>;
    static_assert(is_like_tuple_v<ResultTuple>);

    const auto read_json_task = [](const std::filesystem::path& task_path) {
        auto json_task = json{};
        std::ifstream{task_path} >> json_task;
        return json_task;
    };
    const auto json_task_to_args_tuple = [](const json& json_task) {
        return vector_of_args_to_tuple<Args...>(
            std::vector<TaskArg>(json_task));
    };

    const auto result = std::apply(
        execute, json_task_to_args_tuple(read_json_task(args.task_path)));

    auto json_task_out = json::array();
    for_each_in_tuple(
        [&](const auto& i) { json_task_out.push_back(to_arg(i, {})); }, result);
    std::ofstream{args.task_out_path} << json_task_out;
}

} // namespace gwasm::detail

#endif // GWASM_DETAIL_EXECUTE_HPP
