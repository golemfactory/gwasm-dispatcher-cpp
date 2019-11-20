#include "../../include/gwasm/detail/task_def.hpp"
#include "../../include/gwasm/error.hpp"

#include <stdexcept>

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
    if (!j.is_object()) {
        throw gwasm::GwasmError{"error parsing task arg: json not an object"};
    }

    if (auto json_meta = j.find("meta"); json_meta != j.end()) {
        task_arg = gwasm::detail::TaskArgMeta{*json_meta};
    }
    else if (auto json_blob = j.find("blob"); json_blob != j.end()) {
        task_arg = gwasm::detail::TaskArgBlob{json_blob->get<std::string>()};
    }
    else if (auto json_output = j.find("output"); json_output != j.end()) {
        task_arg =
            gwasm::detail::TaskArgOutput{json_output->get<std::string>()};
    }
    else {
        throw gwasm::GwasmError{
            "invalid task arg json object: no meta, blob or output found"};
    }
}

} // namespace nlohmann
