#ifndef GWASM_DETAIL_STEP_ARGS_HPP
#define GWASM_DETAIL_STEP_ARGS_HPP

#include <filesystem>
#include <variant>

#include "../args.hpp"

namespace gwasm::detail {

struct SplitStepArgs
{
    std::filesystem::path work_dir;
    Args args;
};

struct ExecuteStepArgs
{
    std::filesystem::path task_path;
    std::filesystem::path task_out_path;
};

struct MergeStepArgs
{
    std::filesystem::path tasks_path;
    std::filesystem::path tasks_out_path;
    Args args;
};

std::variant<SplitStepArgs, ExecuteStepArgs, MergeStepArgs, int>
parse_args(int argc, char* argv[]);

} // namespace gwasm::detail

#endif // GWASM_DETAIL_ARGS_HPP
