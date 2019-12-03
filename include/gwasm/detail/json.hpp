#ifndef GWASM_DETAIL_JSON_HPP
#define GWASM_DETAIL_JSON_HPP

#include <filesystem>

#include <nlohmann/json.hpp>

namespace gwasm::detail {

using json = nlohmann::json;

json
read_json(const std::filesystem::path& path);

} // namespace gwasm::detail

#endif // GWASM_DETAIL_JSON_HPP
