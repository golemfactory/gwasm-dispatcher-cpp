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
