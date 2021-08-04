#include "row.hh"

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
