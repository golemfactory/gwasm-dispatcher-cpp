#ifndef GWASM_DETAIL_STEP_ARGS_HPP
#define GWASM_DETAIL_STEP_ARGS_HPP

#include <filesystem>
#include <string>
#include <variant>

namespace gwasm::detail {

struct SplitStepArgs
{
    int argc;
    char** argv;
    std::filesystem::path work_dir;
};

struct ExecuteStepArgs
{
    std::filesystem::path task_path;
    std::filesystem::path task_out_path;
};

struct MergeStepArgs
{
    int argc;
    char** argv;
    std::filesystem::path tasks_path;
    std::filesystem::path tasks_out_path;
};

struct Error
{
    std::string message;
    int exit_code = 1;
};

std::variant<SplitStepArgs, ExecuteStepArgs, MergeStepArgs, Error>
parse_args(int argc, char* argv[]);

} // namespace gwasm::detail

#endif // GWASM_DETAIL_ARGS_HPP
