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
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/detail/json.hpp"
#include "../include/gwasm/detail/utils.hpp"
#include "../include/gwasm/dispatcher.hpp"

#include "args.hpp"
#include "my_functions.hpp"
#include "temp_dir_fixture.hpp"
#include "utils.hpp"

using json = nlohmann::json;

using gwasm::detail::for_each_in_tuple;

namespace {

std::string
make_task_json_filename(int i)
{
    auto filename = std::ostringstream{};
    filename << "task_" << i++ << ".json";
    return filename.str();
}

std::string
make_task_out_json_filename(int i)
{
    auto filename = std::ostringstream{};
    filename << "task_out_" << i++ << ".json";
    return filename.str();
}

int
run(int argc, char* argv[])
{
    return gwasm::run(argc, argv, my_splitter, my_executor, my_merger);
}

} // namespace

BOOST_FIXTURE_TEST_CASE(dispatcher, TempDirFixture)
{
    const auto input_dir = temp_dir / "in";
    const auto output_dir = temp_dir / "out";
    std::filesystem::create_directory(input_dir);
    std::filesystem::create_directory(output_dir);
    const auto result_path = temp_dir / "result.txt";

    // when
    {
        // call split
        auto args = Args{
            {"this_exec", "split", input_dir, result_path, "arg1", "arg2"}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
    }
    BOOST_TEST_CHECKPOINT("splitted");
    {
        // split tasks.json
        const auto tasks = gwasm::detail::read_json(input_dir / "tasks.json");
        int i = 0;
        for (const auto& task : tasks) {
            std::ofstream{input_dir / make_task_json_filename(i++)} << task;
        }
        BOOST_REQUIRE_EQUAL(i, 2);
    }
    BOOST_TEST_CHECKPOINT("splitted tasks.json");
    for (int i = 0; i < 2; ++i) {
        // call exec
        auto args = Args{{"this_exec",
                          "exec",
                          input_dir / make_task_json_filename(i),
                          output_dir / make_task_out_json_filename(i)}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
        BOOST_TEST_CHECKPOINT("executed task " << i);
    }
    {
        // merge tasks_out.json
        auto tasks_out = json::array();
        for (int i = 0; i < 2; ++i) {
            auto task = gwasm::detail::read_json(
                output_dir / make_task_out_json_filename(i));
            tasks_out.push_back(std::move(task));
        }
        std::ofstream{output_dir / "tasks_out.json"} << std::move(tasks_out);
    }
    BOOST_TEST_CHECKPOINT("merged tasks_out.json");
    {
        // call merge
        auto args = Args{{"this_exec",
                          "merge",
                          input_dir / "tasks.json",
                          output_dir / "tasks_out.json",
                          "--",
                          result_path,
                          "arg1",
                          "arg2"}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
    }
    BOOST_TEST_CHECKPOINT("merged");

    // then
    const auto result = read_file_contents(result_path);
    const auto expected_result = "arg1"
                                 "arg2"
                                 "arg1"
                                 "0"
                                 "1"
                                 "arg10"
                                 "arg2"
                                 "1"
                                 "2"
                                 "arg21";
    BOOST_CHECK_EQUAL(result, expected_result);
}
