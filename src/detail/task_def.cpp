#include "../../include/gwasm/detail/task_def.hpp"

namespace gwasm::detail {

TaskArg
into_arg(const Blob& blob, const std::filesystem::path&)
{
    return TaskArgBlob{blob.m_path.string()};
}

} // namespace gwasm::detail
