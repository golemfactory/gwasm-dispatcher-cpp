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

#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/merge.hpp"
#include "../include/gwasm/detail/utils.hpp"

#include "args.hpp"
#include "my_functions.hpp"
#include "temp_dir_fixture.hpp"
#include "utils.hpp"

using json = nlohmann::json;
using gwasm::detail::for_each_in_tuple;

BOOST_FIXTURE_TEST_CASE(merge, TempDirFixture)
{
    // given
    const auto input_dir = temp_dir / "in";
    const auto output_dir = temp_dir / "out";
    std::filesystem::create_directory(input_dir);
    std::filesystem::create_directory(output_dir);
    const auto result_path = temp_dir / "result.txt";

    auto args = Args{{result_path.string(), "arg1", "arg2"}};
    const auto merge_args = gwasm::detail::MergeStepArgs{
        .argc = args.c(),
        .argv = args.v(),
        .tasks_path = input_dir / "tasks.json",
        .tasks_out_path = output_dir / "tasks_out.json",
    };

    const auto blob1_name = "blob1.bin";
    const auto blob1_path = input_dir / blob1_name;
    const auto blob2_name = "blob2.bin";
    const auto blob2_path = input_dir / blob2_name;
    const auto output1_name = "output1.bin";
    const auto output1_path = output_dir / output1_name;
    const auto output2_name = "output2.bin";
    const auto output2_path = output_dir / output2_name;

    std::ofstream{blob1_path} << "blob1";
    std::ofstream{blob2_path} << "blob2";
    std::ofstream{output1_path} << "output1";
    std::ofstream{output2_path} << "output2";

    std::ofstream{merge_args.tasks_path} << json::array({
        json::array({
            json::object({{"blob", blob1_name}}),
            json::object({{"meta", 1}}),
            json::object({{"output", output1_name}}),
        }),
        json::array({
            json::object({{"blob", blob2_name}}),
            json::object({{"meta", 2}}),
            json::object({{"output", output2_name}}),
        }),
    });

    std::ofstream{merge_args.tasks_out_path} << json::array({
        json::array({
            json::object({{"meta", 2}}),
            json::object({{"blob", output1_name}}),
        }),
        json::array({
            json::object({{"meta", 4}}),
            json::object({{"blob", output2_name}}),
        }),
    });

    // when
    gwasm::detail::merge_step<std::tuple<gwasm::Blob, int, gwasm::Output>,
                              std::tuple<int, gwasm::Blob>>(my_merger,
                                                            merge_args);

    // then
    const auto result = read_file_contents(std::ifstream{result_path});
    const auto expected_result = "arg1"
                                 "arg2"
                                 "blob1"
                                 "1"
                                 "2"
                                 "output1"
                                 "blob2"
                                 "2"
                                 "4"
                                 "output2";
    BOOST_CHECK_EQUAL(result, expected_result);
}
