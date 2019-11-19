#ifndef GWASM_DETAIL_TASK_DEF_HPP
#define GWASM_DETAIL_TASK_DEF_HPP

#include <filesystem>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

#include "../blob.hpp"
#include "utils.hpp"

namespace gwasm::detail {

using json = nlohmann::json;

struct TaskArgMeta
{
    json value;
};

struct TaskArgBlob
{
    std::string path;
};

struct TaskArgOutput
{
    std::string path;
};

using TaskArg = std::variant<TaskArgMeta, TaskArgBlob, TaskArgOutput>;

template <typename T>
TaskArg
to_arg(const T& v, const std::filesystem::path&)
{
    return TaskArgMeta{v};
}

TaskArg
to_arg(const Blob& blob, const std::filesystem::path&);

TaskArg
to_arg(const Output& output, const std::filesystem::path&);

template <typename T>
void
from_arg(T& v, const TaskArg& arg, const std::filesystem::path&)
{
    v = std::get<TaskArgMeta>(arg).value;
}

void
from_arg(Blob& blob, const TaskArg& arg, const std::filesystem::path&);

void
from_arg(Output& output, const TaskArg& arg, const std::filesystem::path&);

} // namespace gwasm::detail

namespace nlohmann {

template <>
struct adl_serializer<gwasm::detail::TaskArg>
{
    static void to_json(json& j, const gwasm::detail::TaskArg& task_arg);
    static void from_json(const json& j, gwasm::detail::TaskArg& task_arg);
};

} // namespace nlohmann

#endif // GWASM_DETAIL_TASK_DEF_HPP
