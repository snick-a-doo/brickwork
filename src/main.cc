#include "brickwork.hh"
#include "draw.hh"
#include "row.hh"

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

void ascii_wall(Row const& base, std::vector<Row>& bw_rows)
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

    std::vector<int> pattern;
    for (int i = 1; i < argc; ++i)
        pattern.push_back(std::atoi(argv[i]));

    Row base(0, pattern);
    std::vector<Row> bw_rows = generate(base);
    std::cout << bw_rows.size() << std::endl;
    svg_wall("brickwork.svg", 600, base, bw_rows, 4);
    ascii_wall(base, bw_rows);
    return 0;
}
