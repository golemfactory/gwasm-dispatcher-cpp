#include "my_functions.hpp"

#include <stdexcept>

#include "../include/gwasm/detail/utils.hpp"

#include "utils.hpp"

std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>
my_splitter(int argc, char* argv[], gwasm::SplitContext& context)
{
    auto out = std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>{};
    for (int i = 0; i < argc; ++i) {
        auto output = context.new_blob();
        {
            output.open() << argv[i];
        }
        out.push_back({std::move(output).to_blob(), i, context.new_blob()});
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
