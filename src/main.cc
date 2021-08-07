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
    if (argc < 2)
    {
        std::cerr << usage;
        return 1;
    }

    std::vector<int> pat1;
    std::vector<int> pat2;
    auto i {1};
    for (; i < argc && std::atoi(argv[i]) > 0; ++i)
        pat1.push_back(std::atoi(argv[i]));
    for (++i; i < argc; ++i)
        pat2.push_back(std::atoi(argv[i]));

    auto walls = generate(Row{0, pat1}, pat2.empty() ? Row{0, pat1} : Row{0, pat2});
    svg_walls("brickwork.svg", 600, walls, 8);
    return 0;
}
