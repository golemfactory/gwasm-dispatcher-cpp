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

// Split == Callable<Iterable<Tuple<T...>>(const Args&, SplitContext&)>
template <typename Split>
void
split_step(Split&& split, const SplitStepArgs& args)
{
    static_assert(std::is_invocable_v<Split, const Args&, SplitContext&>);
    using Iterable = std::invoke_result_t<Split, const Args&, SplitContext&>;
    static_assert(is_iterable_v<Iterable>);
    using Tuple = typename Iterable::value_type;
    static_assert(is_like_tuple_v<Tuple>);

    auto work_item_descs = [&] {
        auto split_context = SplitContext{args.work_dir};
        return split(args.args, split_context);
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
