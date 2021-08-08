#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "brickwork.hh"
#include "wall.hh"

#include <cassert>

bool test_is_brickwork(Row const& r1, Row const& r2)
{
    if (r1.pattern().empty() || r2.pattern().empty())
        return false;
    std::string s1(r1);
    std::string s2(r2);
    assert(!s1.empty());
    assert(s1.size() == s2.size());
    // Look for any matching breaks after the 1st element
    for (std::size_t i = 1; i < s1.size(); ++i)
        if (s1[i] == '|' && s2[i] == '|')
            return false;
    return true;
}

void CHECK_BW(Wall const& wall, bool is = true)
{
    for (std::size_t i{0}; i < wall.size(); ++i)
    {
        auto const& r1 = wall[i];
        auto const& r2 = wall[(i + 1) % wall.size()];
        auto test_bw = test_is_brickwork(r1, r2);
        auto bw = is_brickwork(r1, r2);
        CHECK(test_bw == bw);
        CHECK(test_bw == is);
        if (test_bw != bw || test_bw != is)
        {
            std::cerr << r1 << '\n' << r2 << '\n';
            return;
        }
    }
}

TEST_CASE("empty rows")
{
    Row row1(0, {});
    Row row2(1, {});
    Row row3(1, {2, 3});
    CHECK_BW({row1, row2}, false);
    CHECK_BW({row1, row3}, false);
    CHECK_BW({row3, row2}, false);
}

TEST_CASE("same row")
{
    Row row1(1, {2,3});
    Row row2(1, {2,3});
    CHECK_BW({row1, row2}, false);
}

TEST_CASE("too much offset")
{
    CHECK_BW({Row(0,{2}), Row(0,{2})}, false); // no offset
    CHECK_BW({Row(0,{2}), Row(2,{2})}, false); // not interlocked
    CHECK_BW({Row(2,{2}), Row(0,{2})}, false); // not interlocked
    CHECK_BW({Row(0,{3,1}), Row(1,{3,1})}, false);
    CHECK_BW({Row(1,{1,4}), Row(3,{4,1})}, false);
}

TEST_CASE("equal offset")
{
    // Any rows with the same offset eventually line up.
    int const low = 2;
    int const high = 7;
    for (int b = 0; b < high; ++b)
        for (int i = low; i < high; ++i)
            for (int j = low; j < high; ++j)
                for (int k = low; k < high; ++k)
                    for (int l = low; l < high; ++l)
                        CHECK_BW({Row(0,{i,j}), Row(0,{k,l})}, false);
}

TEST_CASE("staggered")
{
    auto const low {2};
    auto const high {7};
    for (auto i = low; i < high; ++i)
        for (auto b = low; b < i; ++b)
            CHECK_BW({Row(0,{i}), Row(b, {i})});
}

TEST_CASE("flemish bond")
{
    CHECK_BW({Row{0, {3,1}}, Row{2, {3,1}}});
    CHECK_BW({Row{0, {3,1}}, Row{1, {1,3}}});
}

TEST_CASE("generate")
{
    std::vector<Wall> walls;
    SUBCASE("width 2")
    {
        walls = generate(2, 1, 2);
        CHECK(walls.size() == 1);
        CHECK(walls[0][1] == Row{1, {2}});
    }
    SUBCASE("width 4")
    {
        walls = generate(2, 1, 4);
        // 1 wall with width 2 + 2 with width 3 + 3 with width 4
        CHECK(walls.size() == 6);
    }
    SUBCASE("2 from 1 to 3")
    {
        walls = generate(2, 2, 3);
        CHECK(walls.size() == 9);
    }
    SUBCASE("3 from 1 to 3")
    {
        walls = generate(2, 3, 3);
        CHECK(walls.size() == 30);
    }
    SUBCASE("3 rows, 2 bricks from 1 to 4")
    {
        walls = generate(3, 2, 4);
        CHECK(walls.size() == 26);
    }

    for (auto const& w : walls)
        CHECK_BW(w);
}
