#ifndef GWASM_ERROR_HPP
#define GWASM_ERROR_HPP

#include <stdexcept>
#include <string>

namespace gwasm {

class GwasmError : public std::runtime_error
{
private:
    using Base = std::runtime_error;

public:
    using Base::Base;
};

} // namespace gwasm

#endif // GWASM_ERROR_HPP
