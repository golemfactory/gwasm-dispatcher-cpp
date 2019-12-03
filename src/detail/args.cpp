#include "../../include/gwasm/detail/step_args.hpp"

namespace gwasm::detail {

std::variant<SplitStepArgs, ExecuteStepArgs, MergeStepArgs, int>
parse_args(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}

} // namespace gwasm::detail
