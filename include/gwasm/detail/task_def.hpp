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
into_arg(const T& v, const std::filesystem::path&)
{
    return TaskArgMeta{v};
}

TaskArg
into_arg(const Blob& blob, const std::filesystem::path&);

} // namespace gwasm::detail

namespace nlohmann {

template <>
struct adl_serializer<gwasm::detail::TaskArg>
{
    static void to_json(json& j, const gwasm::detail::TaskArg& task_arg)
    {
        std::visit(gwasm::detail::overloaded{
                       [&](const gwasm::detail::TaskArgMeta& meta) {
                           j["meta"] = meta.value;
                       },
                       [&](const gwasm::detail::TaskArgBlob& blob) {
                           j["blob"] = blob.path;
                       },
                       [&](const gwasm::detail::TaskArgOutput& output) {
                           j["output"] = output.path;
                       }},
                   task_arg);
    }
};

} // namespace nlohmann

#endif // GWASM_DETAIL_TASK_DEF_HPP
