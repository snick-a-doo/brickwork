#include "brickwork.hh"
#include "draw.hh"
#include "row.hh"

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    std::vector<int> pattern;
    for (int i = 1; i < argc; ++i)
        pattern.push_back(std::atoi(argv[i]));

    Row base(0, pattern);
    std::vector<Row> bw_rows = generate(base);
    draw_wall("brickwork.svg", 600, base, bw_rows, 4);
    return 0;
}
