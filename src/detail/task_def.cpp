/*
Copyright 2019 Golem Factory GmbH

This file is part of gwasm dispatcher cpp.

gwasm dispatcher cpp is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gwasm dispatcher cpp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with gwasm dispatcher cpp. If not, see <https://www.gnu.org/licenses/>.
*/

#include "../../include/gwasm/detail/task_def.hpp"
#include "../../include/gwasm/error.hpp"

#include <filesystem>
#include <stdexcept>

namespace gwasm::detail {

TaskArg
to_arg(const Blob& blob, const std::filesystem::path& base)
{
    return TaskArgBlob{blob.m_absolute_path.lexically_relative(base)};
}

TaskArg
to_arg(const Output& output, const std::filesystem::path& base)
{
    return TaskArgOutput{output.m_absolute_path.lexically_relative(base)};
}

void
from_arg(Blob& blob, const TaskArg& arg, const std::filesystem::path& base)
{
    blob = Blob{base / std::get<TaskArgBlob>(arg).relative_path};
}

void
from_arg(Output& output, const TaskArg& arg, const std::filesystem::path& base)
{
    output = Output{base / std::get<TaskArgOutput>(arg).relative_path};
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
                       j["blob"] = blob.relative_path;
                   },
                   [&](const gwasm::detail::TaskArgOutput& output) {
                       j["output"] = output.relative_path;
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
