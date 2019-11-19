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

namespace nlohmann {

void
adl_serializer<gwasm::detail::TaskArg>::to_json(
    json& j,
    const gwasm::detail::TaskArg& task_arg)
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

void
adl_serializer<gwasm::detail::TaskArg>::from_json(
    const json& j,
    gwasm::detail::TaskArg& task_arg)
{
    std::visit(
        gwasm::detail::overloaded{[&](gwasm::detail::TaskArgMeta& meta) {
                                      meta.value = j.at("meta");
                                  },
                                  [&](gwasm::detail::TaskArgBlob& blob) {
                                      j.at("blob").get_to(blob.path);
                                  },
                                  [&](gwasm::detail::TaskArgOutput& output) {
                                      j.at("output").get_to(output.path);
                                  }},
        task_arg);
}

} // namespace nlohmann
