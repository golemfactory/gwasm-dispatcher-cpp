#ifndef GWASM_TEST_MY_FUNCTIONS_HPP
#define GWASM_TEST_MY_FUNCTIONS_HPP

#include <tuple>
#include <vector>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/split.hpp"

std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>
my_splitter(int argc, char* argv[], gwasm::SplitContext& context);

std::tuple<int, gwasm::Blob>
my_executor(gwasm::Blob&& blob, int i, gwasm::Output&& output);

void
my_merger(int argc,
          char* argv[],
          std::vector<std::pair<std::tuple<gwasm::Blob, int, gwasm::Output>,
                                std::tuple<int, gwasm::Blob>>>&& results);

#endif // GWASM_TEST_MY_FUNCTIONS_HPP
