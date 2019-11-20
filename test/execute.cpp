#include <fstream>
#include <string>
#include <tuple>

#include <boost/test/unit_test.hpp>

#include <nlohmann/json.hpp>

#include "../include/gwasm/blob.hpp"
#include "../include/gwasm/detail/execute.hpp"

#include "temp_dir_fixture.hpp"

using json = nlohmann::json;

namespace {

std::string
read_file_contents(std::ifstream&& file)
{
    file.seekg(0, std::ios::end);
    const auto size = file.tellg();
    auto buffer = std::string(size, '\0');
    file.seekg(0);
    file.read(buffer.data(), size);
    return buffer;
}

} // namespace

BOOST_FIXTURE_TEST_CASE(execute, TempDirFixture)
{
    // given
    const auto my_executor =
        [](const gwasm::Blob& blob, const int i, const gwasm::Output& output) {
            const auto blob_contents = read_file_contents(blob.open());
            output.open() << blob_contents << i;
            return std::make_tuple(i, output);
        };

    const auto execute_args = gwasm::detail::ExecuteArgs{
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
    gwasm::detail::
        execute_step<decltype(my_executor)&, gwasm::Blob, int, gwasm::Output>(
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
            json::object({{"meta", 5}}),
            json::object({{"output", output_path}}),
        });
        BOOST_CHECK_EQUAL(task_out, expected_task_out);
    }
}
