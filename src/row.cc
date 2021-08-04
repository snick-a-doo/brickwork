#include "row.hh"

Row::operator std::string() const
{
    static auto constexpr len {80};
    auto x {m_offset};
    std::string gen_rows(len, ' ');
    gen_rows[0] = '|';
    gen_rows[x] = '|';
    do
    {
        for (auto dx : m_pattern)
            if ((x += dx) < len)
                gen_rows[x] = '|';
    } while (x > m_offset && x < len);
    return gen_rows;
}

std::ostream& operator <<(std::ostream& os, Row const& row)
{
    return os << std::string(row);
}
