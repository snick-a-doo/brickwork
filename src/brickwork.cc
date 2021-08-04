#include "brickwork.hh"

#include <algorithm>
#include <numeric>

std::vector<Row> generate(Row const& base)
{
    std::vector<Row> gen_rows;

    // Permute indices rather than elements so we start with the passed-in vector.
    std::vector<std::size_t> indices(base.pattern().size());
    std::iota(indices.begin(), indices.end(), 0u);
    do
    {
        // Don't generate a row with an offset longer than the base row's 1st brick.
        for (auto offset {0}; offset < base.pattern().front(); ++offset)
        {
            // Make the permuted pattern of lengths.
            std::vector<int> pat;
            for (auto i : indices)
                pat.push_back(base.pattern()[i]);

            Row row {offset, pat};
            if (is_brickwork(base, row))
                gen_rows.push_back(row);
        }
    } while(std::next_permutation(indices.begin(), indices.end()));

    // Put the rows in ascending order by offset, 1st brick, 2nd brick,... and remove
    // duplicates.
    std::sort(gen_rows.begin(), gen_rows.end());
    auto last {std::unique(gen_rows.begin(), gen_rows.end())};
    gen_rows.erase(last, gen_rows.end());
    return gen_rows;
}

bool is_brickwork(Row const& lower, Row const& upper)
{
    auto const b1 {lower.offset()};
    auto const& p1 {lower.pattern()};
    auto const b2 {upper.offset()};
    auto const& p2 {upper.pattern()};

    if (p1.empty() || p2.empty())
        return false;

    // The two-row pattern repeats after the least common multiple of the total lengths of
    // the pattern. If there are no aligned gaps by that point, there will be no aligned
    // gaps.
    auto const sum1 {std::accumulate(p1.begin(), p1.end(), 0)};
    auto const sum2 {std::accumulate(p2.begin(), p2.end(), 0)};
    auto const n_to_check {std::lcm(sum1, sum2)};

    auto x1 {b1};
    auto x2 {b2};
    auto i1 {0u};
    auto i2 {0u};
    // Keep going until the rows line up or we've checked enough to know that they won't.
    while (x1 != x2 && x1 < b1 + n_to_check && x2 < b2 + n_to_check)
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
