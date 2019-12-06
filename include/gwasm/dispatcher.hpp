#ifndef GWASM_DISPATCHER_HPP
#define GWASM_DISPATCHER_HPP

#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "blob.hpp"
#include "detail/execute.hpp"
#include "detail/merge.hpp"
#include "detail/split.hpp"
#include "detail/step_args.hpp"
#include "detail/utils.hpp"

namespace gwasm {

// Split == Callable<Iterable<SplitResultTuple>(int, char**, SplitContext&)>
// Execute == Callable<ExecuteResultTuple(const SplitResultTupleArgs&...)>
// Merge = Callable<void(
//     int, char**,
//     std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>&&)>
template <typename Split, typename Execute, typename Merge>
int
run(int argc, char* argv[], Split split, Execute execute, Merge merge)
{
    // check plit
    static_assert(std::is_invocable_v<Split, int, char**, SplitContext&>);
    using SplitResultIterable =
        std::invoke_result_t<Split, int, char**, SplitContext&>;
    static_assert(detail::is_iterable_v<SplitResultIterable>);
    using SplitResultTuple = typename SplitResultIterable::value_type;
    static_assert(detail::is_like_tuple_v<SplitResultTuple>);

    // check execute
    static_assert(detail::is_applicable_v<Execute, SplitResultTuple>);
    using ExecuteResultTuple =
        detail::apply_result_t<Execute, SplitResultTuple>;
    static_assert(detail::is_like_tuple_v<ExecuteResultTuple>);

    // check merge
    static_assert(
        std::is_invocable_v<
            Merge,
            int,
            char**,
            std::vector<std::pair<SplitResultTuple, ExecuteResultTuple>>&&>);

    // do it!
    const auto args = detail::parse_args(argc, argv);
    auto exit_code = int{0};
    std::visit(detail::overloaded{
                   [split](const detail::SplitStepArgs& args) {
                       detail::split_step(split, args);
                   },
                   [execute](const detail::ExecuteStepArgs& args) {
                       detail::execute_step<SplitResultTuple>(execute, args);
                   },
                   [merge](const detail::MergeStepArgs& args) {
                       detail::merge_step<SplitResultTuple, ExecuteResultTuple>(
                           merge, args);
                   },
                   [&](const detail::Error& error) {
                       std::cerr << "Error: " << error.message << '\n';
                       exit_code = error.exit_code;
                   }},
               args);
    return exit_code;
}

} // namespace gwasm

#endif // GWASM_DISPATCHER_HPP
