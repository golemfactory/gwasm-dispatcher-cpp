#ifndef GWASM_SPLIT_HPP
#define GWASM_SPLIT_HPP

#include <filesystem>

#include "blob.hpp"

namespace gwasm {

// this class allocates files for you
class SplitContext
{
private:
    std::filesystem::path m_work_dir;
    int m_id;

public:
    explicit SplitContext(std::filesystem::path work_dir);

    SplitContext(const SplitContext&) = delete;
    SplitContext& operator=(const SplitContext&) = delete;

    Output new_blob();
};

} // namespace gwasm

#endif // GWASM_SPLIT_HPP
