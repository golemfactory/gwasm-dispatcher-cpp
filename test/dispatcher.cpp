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
    // when
    {
        // call split
        auto args =
            Args{{"this_exec", "split", temp_dir, "split_arg1", "split_arg2"}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
    }
    BOOST_TEST_CHECKPOINT("splitted");
    {
        // split tasks.json
        const auto tasks = gwasm::detail::read_json(temp_dir / "tasks.json");
        int i = 0;
        for (const auto& task : tasks) {
            std::ofstream{temp_dir / make_task_json_filename(i++)} << task;
        }
        BOOST_REQUIRE_EQUAL(i, 2);
    }
    BOOST_TEST_CHECKPOINT("splitted tasks.json");
    for (int i = 0; i < 2; ++i) {
        // call exec
        auto args = Args{{"this_exec",
                          "exec",
                          temp_dir / make_task_json_filename(i),
                          temp_dir / make_task_out_json_filename(i)}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
        BOOST_TEST_CHECKPOINT("executed task " << i);
    }
    {
        // merge tasks_out.json
        auto tasks_out = json::array();
        for (int i = 0; i < 2; ++i) {
            auto task = gwasm::detail::read_json(
                temp_dir / make_task_out_json_filename(i));
            tasks_out.push_back(std::move(task));
        }
        std::ofstream{temp_dir / "tasks_out.json"} << std::move(tasks_out);
    }
    BOOST_TEST_CHECKPOINT("merged tasks_out.json");
    const auto result_path = temp_dir / "result.txt";
    {
        // call merge
        auto args = Args{{"this_exec",
                          "merge",
                          temp_dir / "tasks.json",
                          temp_dir / "tasks_out.json",
                          "--",
                          result_path,
                          "merge_arg1",
                          "merge_arg2"}};
        const auto result = run(args.c(), args.v());
        BOOST_REQUIRE_EQUAL(result, 0);
    }
    BOOST_TEST_CHECKPOINT("merged");

    // then
    const auto result = read_file_contents(result_path);
    const auto expected_result = "merge_arg1"
                                 "merge_arg2"
                                 "split_arg1"
                                 "0"
                                 "split_arg10"
                                 "1"
                                 "split_arg10"
                                 "split_arg2"
                                 "1"
                                 "split_arg21"
                                 "2"
                                 "split_arg21";
    BOOST_CHECK_EQUAL(result, expected_result);
}
