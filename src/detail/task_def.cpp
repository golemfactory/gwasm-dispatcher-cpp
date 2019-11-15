#include "../../include/gwasm/detail/task_def.hpp"

namespace gwasm::detail {

TaskArg
into_arg(const Blob& blob, const std::filesystem::path&)
{
    return TaskArgBlob{blob.m_path.string()};
}

TaskArg
into_arg(const Output& output, const std::filesystem::path&)
{
    return TaskArgOutput{output.m_path.string()};
}

} // namespace gwasm::detail
