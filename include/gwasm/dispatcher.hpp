#ifndef GWASM_DISPATCHER_HPP
#define GWASM_DISPATCHER_HPP

#include <functional>
#include <tuple>
#include <variant>
#include <vector>

#include "args.hpp"
#include "blob.hpp"
#include "detail/execute.hpp"
#include "detail/merge.hpp"
#include "detail/split.hpp"
#include "detail/step_args.hpp"
#include "detail/utils.hpp"

namespace gwasm {

// Split == Callable<Iterable<Tuple<T...>>(const Args&, SplitContext&)>
// Execute == Callable<WorkItemResult(const T&...)>
// Merge = Callable<void(
//             const Args&,
//             const std::vector<std::pair<WorkItemDesc, WorkItemResult>>&)>
template <typename Split, typename Execute, typename Merge>
int
run(int argc, char* argv[], Split&& split, Execute&& execute, Merge&& merge)
{
    const auto args = detail::parse_args(argc, argv);
    auto exit_code = int{0};
    return std::visit(
        detail::overloaded{
            [split = std::move(split)](const detail::SplitStepArgs& args) {
                split_step(std::move(split), args);
            },
            [execute =
                 std::move(execute)](const detail::ExecuteStepArgs& args) {
                execute_step(std::move(execute), args);
            },
            [merge = std::move(merge)](const detail::MergeStepArgs& args) {
                merge_step(std::move(merge), args);
            },
            [&](const int new_exit_code) { exit_code = new_exit_code; }},
        args);
    return exit_code;
}

} // namespace gwasm

#endif // GWASM_DISPATCHER_HPP
