#include <filesystem>
#include <set>
#include <tuple>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/split.hpp"

#include "temp_dir_fixture.hpp"

using json = nlohmann::json;

BOOST_FIXTURE_TEST_CASE(split, TempDirFixture)
{
    // given
    const auto my_splitter = [](const gwasm::Args& args,
                                gwasm::SplitContext& context) {
        auto out = std::vector<std::tuple<gwasm::Blob, int, gwasm::Output>>{};
        for (int i = 0; i < 2; ++i) {
            auto output = context.new_blob();
            {
                auto f = output.open();
                for (const auto& arg : args) {
                    f << arg << '\n';
                }
            }
            out.push_back(
                std::tuple{std::move(output).to_blob(), i, context.new_blob()});
        }
        return out;
    };

    const auto split_args = gwasm::detail::SplitArgs{
        .work_dir = temp_dir,
        .args = {"arg1", "arg2"},
    };

    // when
    gwasm::detail::split_step(my_splitter, split_args);

    // then
    {
        const auto expected_files = std::set{
            temp_dir / "tasks.json",
            temp_dir / "000000.bin",
            // temp_dir / "000001.bin",
            temp_dir / "000002.bin",
            // temp_dir / "000003.bin",
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
        const auto actual_json = [&] {
            auto j = json{};
            std::ifstream{temp_dir / "tasks.json"} >> j;
            return j;
        }();
        BOOST_CHECK_EQUAL(expected_json, actual_json);
    }
}
