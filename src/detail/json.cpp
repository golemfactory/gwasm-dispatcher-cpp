#include "../../include/gwasm/detail/json.hpp"

#include <fstream>

namespace gwasm::detail {

json
read_json(const std::filesystem::path& path)
{
    auto json_ = json{};
    std::ifstream{path} >> json_;
    return json_;
}

} // namespace gwasm::detail
