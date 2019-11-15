#include <filesystem>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/split.hpp"

namespace {

using json = nlohmann::json;

struct TempDirFixture
{
    std::filesystem::path temp_dir;

    TempDirFixture()
        : temp_dir{std::filesystem::temp_directory_path() /
                   ("gwasm_dispatcher_cpp_test_split_" +
                    std::to_string(std::random_device{}()))}
    {
        std::filesystem::remove_all(temp_dir);
        std::filesystem::create_directory(temp_dir);
    }

    ~TempDirFixture() { std::filesystem::remove_all(temp_dir); }
};

} // namespace

BOOST_FIXTURE_TEST_SUITE(split, TempDirFixture)

BOOST_AUTO_TEST_CASE(bla)
{
    // given
    const auto my_splitter = [](const gwasm::Args& args,
                                gwasm::SplitContext& context) {
        auto out = std::vector<std::tuple<gwasm::Blob, int>>{};
        for (int i = 0; i < 4; ++i) {
            auto output = context.new_blob();
            {
                auto f = output.open();
                for (const auto& arg : args) {
                    f << arg << '\n';
                }
            }
            out.push_back(std::make_tuple(std::move(output).to_blob(), i));
        }
        return out;
    };

    const auto split_args = gwasm::detail::SplitArgs{
        .work_dir = temp_dir,
        .args = {"arg1", "arg2"},
    };

    // when
    split_step(my_splitter, split_args);

    // then
    {
        const auto expected_files = std::set{
            temp_dir / "tasks.json",
            temp_dir / "000000.bin",
            temp_dir / "000001.bin",
            temp_dir / "000002.bin",
            temp_dir / "000003.bin",
        };
        const auto actual_files =
            std::set(std::filesystem::directory_iterator{temp_dir},
                     std::filesystem::directory_iterator{});
        BOOST_CHECK_EQUAL_COLLECTIONS(expected_files.begin(),
                                      expected_files.end(),
                                      actual_files.begin(),
                                      actual_files.end());
    }
    {
        const auto expected_json = json::array(
            {json::array(
                 {json::object({{"blob", (temp_dir / "000000.bin").string()}}),
                  json::object({{"meta", 0}})}),
             json::array(
                 {json::object({{"blob", (temp_dir / "000001.bin").string()}}),
                  json::object({{"meta", 1}})}),
             json::array(
                 {json::object({{"blob", (temp_dir / "000002.bin").string()}}),
                  json::object({{"meta", 2}})}),
             json::array(
                 {json::object({{"blob", (temp_dir / "000003.bin").string()}}),
                  json::object({{"meta", 3}})})});
        const auto actual_json = [&] {
            auto j = json{};
            std::ifstream{temp_dir / "tasks.json"} >> j;
            return j;
        }();
        BOOST_CHECK_EQUAL(expected_json, actual_json);
    }
}

BOOST_AUTO_TEST_SUITE_END() // split
