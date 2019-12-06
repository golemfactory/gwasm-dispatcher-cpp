/*
Copyright 2019 Golem Factory GmbH

This file is part of gwasm dispatcher cpp.

gwasm dispatcher cpp is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

gwasm dispatcher cpp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with gwasm dispatcher cpp. If not, see <https://www.gnu.org/licenses/>.
*/

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
