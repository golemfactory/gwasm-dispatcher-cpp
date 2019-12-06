#include "args.hpp"

Args::Args(std::vector<std::string>&& args)
    : m_args{std::move(args)}
    , m_c_args{}
{
    m_c_args.reserve(m_args.size() + 1);
    for (auto& s : m_args) {
        m_c_args.push_back(s.data());
    }
    m_c_args.push_back(nullptr);
}

int
Args::c() const
{
    return m_c_args.size() - 1;
}

char**
Args::v()
{
    return m_c_args.data();
}
