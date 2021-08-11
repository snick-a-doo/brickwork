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
        CHECK(walls.size() == 5);
    }
    SUBCASE("2 from 1 to 3")
    {
        walls = generate(2, 2, 3);
        CHECK(walls.size() == 8);
    }
    SUBCASE("3 from 1 to 3")
    {
        walls = generate(2, 3, 3);
        CHECK(walls.size() == 26);
    }
    SUBCASE("4 rows, 2 bricks from 1 to 4")
    {
        walls = generate(4, 2, 4);
        CHECK(walls.size() == 345);
    }

    for (auto const& w : walls)
        CHECK_BW(w);
}
