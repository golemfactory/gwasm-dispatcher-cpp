#ifndef GWASM_TEST_TEMP_DIR_FIXTURE_HPP
#define GWASM_TEST_TEMP_DIR_FIXTURE_HPP

#include <filesystem>

struct TempDirFixture
{
    std::filesystem::path temp_dir;

    TempDirFixture();
    ~TempDirFixture();
};

#endif // GWASM_TEST_TEMP_DIR_FIXTURE_HPP
