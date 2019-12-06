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

#include <filesystem>
#include <set>
#include <tuple>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/json.hpp"
#include "../include/gwasm/detail/split.hpp"

#include "args.hpp"
#include "my_functions.hpp"
#include "temp_dir_fixture.hpp"

using json = nlohmann::json;

BOOST_FIXTURE_TEST_CASE(split, TempDirFixture)
{
    // given
    auto args = Args{{"arg1", "arg2"}};
    const auto split_args = gwasm::detail::SplitStepArgs{
        .argc = args.c(),
        .argv = args.v(),
        .work_dir = temp_dir,
    };

    // when
    gwasm::detail::split_step(my_splitter, split_args);

    // then
    {
        const auto expected_files = std::set{
            temp_dir / "tasks.json",
            temp_dir / "000000.bin",
            temp_dir / "000002.bin",
        };
        const auto actual_files =
            std::set(std::filesystem::directory_iterator{temp_dir},
                     std::filesystem::directory_iterator{});
        BOOST_REQUIRE_EQUAL_COLLECTIONS(expected_files.begin(),
                                        expected_files.end(),
                                        actual_files.begin(),
                                        actual_files.end());
    }
    {
        const auto expected_json = json::array(
            {json::array({json::object({{"blob", temp_dir / "000000.bin"}}),
                          json::object({{"meta", 0}}),
                          json::object({{"output", temp_dir / "000001.bin"}})}),
             json::array(
                 {json::object({{"blob", temp_dir / "000002.bin"}}),
                  json::object({{"meta", 1}}),
                  json::object({{"output", temp_dir / "000003.bin"}})})});
        const auto actual_json =
            gwasm::detail::read_json(temp_dir / "tasks.json");
        BOOST_CHECK_EQUAL(expected_json, actual_json);
    }
}
