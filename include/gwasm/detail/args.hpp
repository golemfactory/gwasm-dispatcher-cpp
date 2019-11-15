#ifndef GWASM_DETAIL_ARGS_HPP
#define GWASM_DETAIL_ARGS_HPP

#include <filesystem>
#include <variant>

#include "../args.hpp"

namespace gwasm::detail {

struct SplitArgs
{
    std::filesystem::path work_dir;
    Args args;
};

struct ExecuteArgs
{
    std::filesystem::path task_path;
    std::filesystem::path task_out_path;
};

struct MergeArgs
{
    std::filesystem::path tasks_path;
    std::filesystem::path tasks_out_path;
    Args args;
};

std::variant<SplitArgs, ExecuteArgs, MergeArgs, int>
parse_args(int argc, char* argv[]);

} // namespace gwasm::detail

#endif // GWASM_DETAIL_ARGS_HPP
