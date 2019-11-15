#include "../include/gwasm/split.hpp"

#include <iomanip>
#include <sstream>

namespace gwasm {

SplitContext::SplitContext(std::filesystem::path work_dir)
    : m_work_dir{work_dir}
    , m_id{0}
{}

Output
SplitContext::new_blob()
{
    auto name = std::ostringstream{};
    name << std::setw(6) << std::setfill('0') << std::hex << m_id++ << ".bin";
    return Output{m_work_dir / name.str()};
}

} // namespace gwasm
