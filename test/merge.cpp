#include <numeric>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/merge.hpp"
#include "../include/gwasm/detail/utils.hpp"

#include "temp_dir_fixture.hpp"
#include "utils.hpp"

namespace {

using json = nlohmann::json;
using gwasm::detail::for_each_in_tuple;

template <typename T>
T
copy(const T& v)
{
    return v;
}

} // namespace

BOOST_FIXTURE_TEST_CASE(merge, TempDirFixture)
{
    // given
    const auto result_path = temp_dir / "result.txt";

    using DescTuple = std::tuple<gwasm::Blob, int, gwasm::Output>;
    using ResultTuple = std::tuple<int, gwasm::Blob>;
    const auto my_merger =
        [&](const gwasm::Args& args,
            std::vector<std::pair<DescTuple, ResultTuple>>&& results) -> void {
        auto out = std::ofstream{result_path};
        for (const auto& arg : args) {
            out << arg;
        }

        const auto task_arg_printer = gwasm::detail::overloaded{
            [&](auto&& v) { out << v; },
            [&](gwasm::Blob&& blob) { out << read_file_contents(blob.open()); },
            [&](gwasm::Output&& output) {
                out << read_file_contents(std::move(output).to_blob().open());
            }};
        for (auto&& result : std::move(results)) {
            for_each_in_tuple(std::move(result), [&](auto&& task_args) {
                for_each_in_tuple(std::move(task_args), task_arg_printer);
            });
        }
    };

    const auto merge_args = gwasm::detail::MergeStepArgs{
        .tasks_path = temp_dir / "tasks.json",
        .tasks_out_path = temp_dir / "tasks_out.json",
        .args = {"arg1", "arg2"},
    };

    const auto blob1_path = temp_dir / "blob1.bin";
    const auto blob2_path = temp_dir / "blob2.bin";
    const auto output1_path = temp_dir / "output1.bin";
    const auto output2_path = temp_dir / "output2.bin";

    std::ofstream{blob1_path} << "blob1";
    std::ofstream{blob2_path} << "blob2";
    std::ofstream{output1_path} << "output1";
    std::ofstream{output2_path} << "output2";

    std::ofstream{merge_args.tasks_path} << json::array({
        json::array({
            json::object({{"blob", blob1_path}}),
            json::object({{"meta", 1}}),
            json::object({{"output", output1_path}}),
        }),
        json::array({
            json::object({{"blob", blob2_path}}),
            json::object({{"meta", 2}}),
            json::object({{"output", output2_path}}),
        }),
    });

    std::ofstream{merge_args.tasks_out_path} << json::array({
        json::array({
            json::object({{"meta", 2}}),
            json::object({{"blob", output1_path}}),
        }),
        json::array({
            json::object({{"meta", 4}}),
            json::object({{"blob", output2_path}}),
        }),
    });

    // when
    gwasm::detail::merge_step<decltype(my_merger)&, DescTuple, ResultTuple>(
        my_merger, merge_args);

    // then
    const auto result = read_file_contents(std::ifstream{result_path});
    const auto expected_result = "arg1"
                                 "arg2"
                                 "blob1"
                                 "1"
                                 "output1"
                                 "2"
                                 "output1"
                                 "blob2"
                                 "2"
                                 "output2"
                                 "4"
                                 "output2";
    BOOST_CHECK_EQUAL(result, expected_result);
}
