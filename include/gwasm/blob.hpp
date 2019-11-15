#ifndef GWASM_BLOB_HPP
#define GWASM_BLOB_HPP

#include <filesystem>
#include <fstream>
#include <utility>
#include <variant>

namespace gwasm {

class Blob;
class Output;

namespace detail {
struct TaskArgMeta;
struct TaskArgBlob;
struct TaskArgOutput;
using TaskArg = std::variant<TaskArgMeta, TaskArgBlob, TaskArgOutput>;
TaskArg
into_arg(const Blob&, const std::filesystem::path&);
TaskArg
into_arg(const Output&, const std::filesystem::path&);
} // namespace detail

// read-only file access
class Blob
{
private:
    friend detail::TaskArg detail::into_arg(const Blob&,
                                            const std::filesystem::path&);

    std::filesystem::path m_path;

public:
    explicit Blob(std::filesystem::path&& path);

    std::ifstream open() const;
};

// read-write file access
class Output
{
private:
    friend detail::TaskArg detail::into_arg(const Output&,
                                            const std::filesystem::path&);

    std::filesystem::path m_path;

public:
    explicit Output(std::filesystem::path&& path);

    std::ofstream open() const;

    Blob to_blob() &&;
};

} // namespace gwasm

#endif // GWASM_BLOB_HPP
