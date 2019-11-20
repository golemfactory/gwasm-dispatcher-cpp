#include "temp_dir_fixture.hpp"

#include <random>
#include <string>

TempDirFixture::TempDirFixture()
    : temp_dir{std::filesystem::temp_directory_path() /
               ("gwasm_dispatcher_cpp_test_" +
                std::to_string(std::random_device{}()))}
{
    std::filesystem::remove_all(temp_dir);
    std::filesystem::create_directory(temp_dir);
}

TempDirFixture::~TempDirFixture()
{
    std::filesystem::remove_all(temp_dir);
}
