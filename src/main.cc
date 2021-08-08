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
