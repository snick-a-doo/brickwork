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

#include "brickwork.hh"

#include <algorithm>
#include <cassert>
#include <numeric>

// A multi-place counter with a limited container-like interface.
class Counter
{
public:
    // Initialize all digits of to n-place counter to the low value.
    Counter(int n, int low, int high)
        : m_low{low}, m_high{high}, m_sum{n*low}, m_v(n, low)
    {}

    // Increment the least significant place, possibly carrying or overflowing. This is
    // the only way to change the count.
    Counter& operator++();
    // @return The sum of the digits.
    int sum() const { return m_sum; }
    // @return True if the counter has wrapped around to its initial state.
    bool overflow() const { return m_overflow; }

    // The container interface.
    std::size_t size() const { return m_v.size(); }
    auto operator[](std::size_t i) const { return m_v[i]; }
    auto begin() const { return m_v.begin(); }
    auto end() const { return m_v.end(); }
    auto rbegin() const { return m_v.rbegin(); }
    auto rend() const { return m_v.rend(); }

private:
    int const m_low;  // The lowest value of a digit.
    int const m_high; // The highest value of a digit.
    int m_sum;  // The sum of the digits.
    bool m_overflow{false}; // True if the counter has wrapped.
    std::vector<int> m_v;  // The digits.
};

Counter& Counter::operator++()
{
    // Adjust m_sum to avoid the need to sum over the digits.
    for (auto& x : m_v)
    {
        if (x < m_high)
        {
            ++x;
            ++m_sum;
            return *this;
        }
        x = m_low;
        m_sum -= m_high - m_low;
    }
    m_overflow = true;
    return *this;
}

std::vector<Wall> generate(int n_rows, int n_bricks, int widest_brick)
{
    std::vector<Wall> walls;
    if (n_rows < 2 || n_bricks < 1 || widest_brick < 2)
        return walls;

    // Iterate over a flat vector of all the brick widths.
    for (Counter widths(n_rows*n_bricks, 1, widest_brick); !widths.overflow(); ++widths)
    {
        // Use revere iterators to place most significant (slowest changing) bricks first.
        Wall wall{Row{0, std::vector(widths.rbegin(), widths.rbegin() + n_bricks)}};
        for (auto row_num{1}; row_num < n_rows; ++row_num)
        {
            Row row{row_num % 2,
                std::vector(widths.rbegin() + row_num*n_bricks,
                            widths.rbegin() + (row_num + 1)*n_bricks)};
            // Add this row if it fits with the previous row.
            if (is_brickwork(row, wall.back()))
            {
                wall.push_back(row);
                if (static_cast<int>(wall.size()) == n_rows)
                    break;
            }
        }
        // Add the wall if we found enough rows and the 1st row fits on top of the last
        // row.
        if (static_cast<int>(wall.size()) == n_rows
            && is_brickwork(wall.front(), wall.back()))
            walls.push_back(wall);
    }
    return walls;
}

int num_brickworks(int n_rows, int n_bricks, int widest_brick)
{
    assert(n_rows == 2 && "Calculation has not been generalized.");

    auto is_wall = [](auto const& xs, auto const& ys) {
        auto const d{std::gcd(xs.sum(), ys.sum())};
        auto const i_max{static_cast<int>(xs.size())};
        auto const j_max{static_cast<int>(ys.size())};
        for (auto i{0}, x{0}; i < i_max; x += xs[i++])
            for (auto j{0}, y{0}; j < j_max; y += ys[j++])
                if ((1 - x + y) % d == 0)
                    return false;
        return true;
    };

    auto out{0};
    for(Counter x_widths(n_bricks, 1, widest_brick); !x_widths.overflow(); ++x_widths)
        for(Counter y_widths(n_bricks, 1, widest_brick); !y_widths.overflow(); ++y_widths)
            if (is_wall(x_widths, y_widths))
                ++out;
    return out;
}

bool is_brickwork(Row const& lower, Row const& upper)
{
    if (lower.period() <= 0 || upper.period() <= 0)
        return false;

    // The two-row pattern repeats after the least common multiple of the total lengths of
    // the pattern. If there are no aligned gaps by that point, there will be no aligned
    // gaps.
    auto const n_to_check{std::lcm(lower.period(), upper.period())};
    auto const b1{lower.offset()};
    auto const b2{upper.offset()};

    auto const& p1{lower.pattern()};
    auto const& p2{upper.pattern()};
    auto x1{b1};
    auto x2{b2};
    auto i1{0u};
    auto i2{0u};
    // Keep going until the rows line up or we've checked enough to know that they won't.
    while (x1 != x2 && (x1 < b1 + n_to_check || x2 < b2 + n_to_check))
    {
        // Leapfrog: Advance x1 until it meets or passes x2...
        while (x1 < x2)
            x1 += p1[i1++ % p1.size()];
        // ... then advance x2 until it meets or passes x1.
        while (x2 < x1)
            x2 += p2[i2++ % p1.size()];
    }
    return x1 != x2;
}
