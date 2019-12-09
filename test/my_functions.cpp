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

#include "my_functions.hpp"

#include <stdexcept>

#include "../include/gwasm/detail/utils.hpp"

#include "utils.hpp"

std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>
my_splitter(int argc, char* argv[], gwasm::SplitContext& context)
{
    auto out = std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>{};
    for (int i = 0; i < argc; ++i) {
        auto f = context.new_file();
        {
            f.open() << argv[i];
        }
        out.push_back({std::move(f).to_blob(), i, context.new_file()});
    }
    return out;
}

std::tuple<int, gwasm::Blob>
my_executor(gwasm::Blob&& blob, int i, gwasm::Output&& output)
{
    const auto blob_contents = read_file_contents(blob.open());
    output.open() << blob_contents << i;
    return std::tuple{i + 1, std::move(output).to_blob()};
}

void
my_merger(int argc,
          char* argv[],
          std::vector<std::pair<std::tuple<gwasm::Blob, int, gwasm::Output>,
                                std::tuple<int, gwasm::Blob>>>&& results)
{
    if (argc < 1) {
        throw std::runtime_error{"need at least output file path"};
    }

    auto out = std::ofstream{argv[0]};
    for (int i = 1; i < argc; ++i) {
        out << argv[i];
    }

    const auto task_arg_printer = gwasm::detail::overloaded{
        [&](auto&& v) { out << v; },
        [&](gwasm::Blob&& blob) { out << read_file_contents(blob.open()); },
        [&](gwasm::Output&& output) {
            out << read_file_contents(std::move(output).to_blob().open());
        }};
    for (auto&& result : std::move(results)) {
        gwasm::detail::for_each_in_tuple(
            std::move(result), [&](auto&& task_args) {
                gwasm::detail::for_each_in_tuple(std::move(task_args),
                                                 task_arg_printer);
            });
    }
}
