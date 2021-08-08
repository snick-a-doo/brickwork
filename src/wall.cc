// Copyright © 2021 Sam Varner
//
// This file is part of Brickwork.
//
// Composure is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
//
// Composure is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with Composure.
// If not, see <http://www.gnu.org/licenses/>.

#include "wall.hh"

#include <numeric>

int Row::period() const
{
    return std::accumulate(m_pattern.begin(), m_pattern.end(), 0);
}

Row::operator std::string() const
{
    static auto constexpr len {80};
    auto x {m_offset};
    std::string gen_rows(len, ' ');
    if (period() <= 0)
        return gen_rows;
    gen_rows[0] = '|';
    gen_rows[x] = '|';
    do
    {
        for (auto dx : m_pattern)
            if ((x += dx) < len)
                gen_rows[x] = '|';
    } while (x < len);
    return gen_rows;
}

std::ostream& operator <<(std::ostream& os, Row const& row)
{
    return os << std::string(row);
}
