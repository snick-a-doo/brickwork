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
#include "draw.hh"
#include "wall.hh"

#include <iostream>
#include <vector>

static constexpr auto usage {
    "Usage: brickwork l1 l2 ...\n"
    "\twhere l1 l2 ... are the lengths bricks repeated in the bottom row of a wall.\n"
    "\n"
    "Output: An SVG file 'brickwork.svg' that displays the unique brick patterns that\n"
    "\tcan be formed by permuting the base pattern and applying an offset shorter than\n"
    "\tthe first brick in the base pattern.\n"
    "\n"};

void ascii_wall(Row const& base, Wall& bw_rows)
{
    for (auto const& row : bw_rows)
        std::cout << base << '\n' << row << "\n\n";
}

int main(int argc, char* argv[])
{
    auto n_rows{2};
    auto n_bricks{2};
    auto widest_brick{2};
    if (argc > 1)
        n_rows = std::atoi(argv[1]);
    if (argc > 2)
        n_bricks = std::atoi(argv[2]);
    if (argc > 3)
        widest_brick = std::atoi(argv[3]);

    auto walls {generate(n_rows, n_bricks, widest_brick)};
    std::cout << walls.size() << std::endl;
    svg_walls("brickwork.svg", 600, walls, 8);
    return 0;
}
