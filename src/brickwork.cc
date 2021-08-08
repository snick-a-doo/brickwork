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

    // True if contiguous groups of n elements sum to the same value. The size of v must
    // be a multiple of n.
    auto sum_equal = [](std::vector<int> const& v, int n) {
        assert(!v.empty() && v.size() % n == 0);
        auto sum {std::accumulate(v.cbegin(), v.cbegin() + n, 0)};
        for (auto it {v.cbegin() + n}; it != v.cend(); it += n)
            if (std::accumulate(it, it + n, 0) != sum)
                return false;
        return true;
    };

    // Iterate over a flat vector of all the brick widths.
    std::vector<int> widths(n_rows*n_bricks, 1);
    do
    {
        // If total brick length isn't the same in all, the rows will get out of sync
        // because of the different number of gaps per unit length.
        if (!sum_equal(widths, n_bricks))
            continue;
        std::vector<int> offsets(n_rows - 1, 1);
        do
        {
            Wall wall{Row{0, std::vector(widths.cbegin(), widths.cbegin() + n_bricks)}};
            for (auto row_num{1}; row_num < n_rows; ++row_num)
            {
                Row row{offsets[row_num - 1],
                    std::vector(widths.cbegin() + row_num*n_bricks,
                                widths.cbegin() + (row_num + 1)*n_bricks)};
                // Add this row if it fits with the previous row.
                if (is_brickwork(row, wall.back()))
                {
                    wall.push_back(row);
                    if (static_cast<int>(wall.size()) == n_rows)
                        break;
                }
            }
            // Add the wall if we found enough rows and the 1st row its on top of the last
            // row.
            if (static_cast<int>(wall.size()) == n_rows
                && is_brickwork(wall.front(), wall.back()))
                walls.push_back(wall);
        } while (increment(offsets, 1, widths.front() - 1));
    } while (increment(widths, 1, widest_brick));
    return walls;
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
