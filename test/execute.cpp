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

#include <fstream>
#include <string>
#include <tuple>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/execute.hpp"

#include "my_functions.hpp"
#include "temp_dir_fixture.hpp"
#include "utils.hpp"

using json = nlohmann::json;

BOOST_FIXTURE_TEST_CASE(execute, TempDirFixture)
{
    // given
    const auto execute_args = gwasm::detail::ExecuteStepArgs{
        .task_path = temp_dir / "task.json",
        .task_out_path = temp_dir / "task_out.json",
    };

    const auto blob_path = temp_dir / "blob.bin";
    const auto output_path = temp_dir / "output.bin";

    std::ofstream{blob_path} << "Hello world!";

    std::ofstream{execute_args.task_path} << json::array({
        json::object({{"blob", blob_path}}),
        json::object({{"meta", 5}}),
        json::object({{"output", output_path}}),
    });

    // when
    gwasm::detail::execute_step<std::tuple<gwasm::Blob, int, gwasm::Output>>(
        my_executor, execute_args);

    // then
    {
        BOOST_REQUIRE(std::filesystem::is_regular_file(output_path));
        const auto output = read_file_contents(std::ifstream{output_path});
        BOOST_CHECK_EQUAL(output, "Hello world!5");
    }
    {
        BOOST_REQUIRE(
            std::filesystem::is_regular_file(execute_args.task_out_path));
        const auto task_out = [&] {
            auto task_out = json{};
            std::ifstream{execute_args.task_out_path} >> task_out;
            return task_out;
        }();
        const auto expected_task_out = json::array({
            json::object({{"meta", 6}}),
            json::object({{"blob", output_path}}),
        });
        BOOST_CHECK_EQUAL(task_out, expected_task_out);
    }
}
