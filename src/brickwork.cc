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

std::vector<Wall> generate(int n_rows, int n_bricks, int widest_brick)
{
    std::vector<Wall> walls;
    if (n_rows < 2 || n_bricks < 1 || widest_brick < 2)
        return walls;

    // Increment the 1st element. Overflow to min and carry if max is exceeded.
    auto increment = [](std::vector<int>& v, int min, int max) {
        for (auto& x : v)
        {
            if (x < max)
            {
                ++x;
                return true;
            }
            x = min;
        }
        return false;
    };

    // Iterate over a flat vector of all the brick widths.
    std::vector<int> widths(n_rows*n_bricks, 1);
    do
    {
        // Use revere iterators to place most significant (slowest changing) bricks first.
        Wall wall{Row{0, std::vector(widths.crbegin(), widths.crbegin() + n_bricks)}};
        for (auto row_num{1}; row_num < n_rows; ++row_num)
        {
            Row row{row_num % 2,
                std::vector(widths.crbegin() + row_num*n_bricks,
                            widths.crbegin() + (row_num + 1)*n_bricks)};
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
    } while (increment(widths, 1, widest_brick));
    return walls;
}

int num_brickworks(int n_rows, int n_bricks, int widest_brick)
{
    assert(n_rows == 2 && "Calculation has not been generalized.");
    assert(n_bricks == 2 && "Calculation has not been generalized.");

    // True if 1-x1, 1+y1, 1-x1+y1 are not multiples of gcd(x1+x2, y1+y2)
    auto is_wall = [](int x1, int x2, int y1, int y2) {
        auto d{std::gcd(x1+x2, y1+y2)};
        for (auto a : {1-x1, 1+y1, 1-x1+y1})
            if (a % d == 0)
                return false;
        return true;
    };

    auto out{0};
    std::vector<int> is(widest_brick);
    std::iota(is.begin(), is.end(), 1);
    for (auto x1 : is)
        for (auto x2 : is)
            for (auto y1 : is)
                for (auto y2 : is)
                    if (is_wall(x1, x2, y1, y2))
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
