#include "../../include/gwasm/detail/args.hpp"

namespace gwasm::detail {

std::variant<SplitArgs, ExecuteArgs, MergeArgs, int>
parse_args(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    return 0;
}

} // namespace gwasm::detail
