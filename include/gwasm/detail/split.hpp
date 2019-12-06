#ifndef GWASM_DETAIL_SPLIT_HPP
#define GWASM_DETAIL_SPLIT_HPP

#include <fstream>
#include <tuple>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include "../split.hpp"
#include "step_args.hpp"
#include "task_def.hpp"
#include "utils.hpp"

namespace gwasm::detail {

using json = nlohmann::json;

// Split == Callable<Iterable<SplitResultTuple>(int, char**, SplitContext&)>
template <typename Split>
void
split_step(Split split, const SplitStepArgs& args)
{
    static_assert(std::is_invocable_v<Split, int, char**, SplitContext&>);
    using SplitResultIterable =
        std::invoke_result_t<Split, int, char**, SplitContext&>;
    static_assert(is_iterable_v<SplitResultIterable>);
    using SplitResultTuple = typename SplitResultIterable::value_type;
    static_assert(is_like_tuple_v<SplitResultTuple>);

    auto work_item_descs = [&] {
        auto split_context = SplitContext{args.work_dir};
        return split(args.argc, args.argv, split_context);
    }();

    auto json_tasks = json::array();
    for (auto&& work_item_desc : std::move(work_item_descs)) {
        auto json_work_item_desc = json::array();
        for_each_in_tuple(std::move(work_item_desc), [&](auto&& i) {
            json_work_item_desc.push_back(to_arg(std::move(i), {}));
        });
        json_tasks.push_back(std::move(json_work_item_desc));
    }

    std::ofstream{args.work_dir / "tasks.json"} << json_tasks;
}

} // namespace gwasm::detail

#endif // GWASM_DETAIL_SPLIT_HPP
