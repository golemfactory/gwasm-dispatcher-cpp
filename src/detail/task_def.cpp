#include "../../include/gwasm/detail/task_def.hpp"

namespace gwasm::detail {

TaskArg
to_arg(const Blob& blob, const std::filesystem::path&)
{
    return TaskArgBlob{blob.m_path.string()};
}

TaskArg
to_arg(const Output& output, const std::filesystem::path&)
{
    return TaskArgOutput{output.m_path.string()};
}

void
from_arg(Blob& blob, const TaskArg& arg, const std::filesystem::path&)
{
    blob = Blob{std::get<TaskArgBlob>(arg).path};
}

void
from_arg(Output& output, const TaskArg& arg, const std::filesystem::path&)
{
    output = Output{std::get<TaskArgOutput>(arg).path};
}

} // namespace gwasm::detail
