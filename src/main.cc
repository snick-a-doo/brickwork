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

#include <getopt.h>

#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

auto constexpr info{
    "Brickwork: Brick pattern genereator\n"
    "Version 0.3.0 © 2021 Sam Varner GPL3\n"
    "https://github.com/snick-a-doo/brickwork\n"
};

auto constexpr usage{
    "\nUsage: Brickwork [options] [courses] [bricks] [max_brick]\n"
    "    -a --ascii   Render ASCII walls to standard output or, if --output or -o is\n"
    "                 specified, a text file.\n"
    "    -c --count   Output the number of walls. Nothing is rendered, even if other\n"
    "                  output-related options are given.\n"
    "    -h --help    Display this message and exit.\n"
    "    -o --output= File name for the rendering sans extension. Defaults to\n"
    "                 'brickwork'. An extension is appended, .svg or .txt, depending\n"
    "                 on other options.\n"
    "\n"
    "    courses      The number of repeated rows of bricks. Must be even.\n"
    "    bricks       The number of repeated bricks in each course.\n"
    "    max_brick    The maximum brick width.\n"
    "\n"
    "If neither --ascii nor --count is given, an SVG image file is produced.\n"
};

struct Options
{
    int n_rows{2};
    int n_bricks{2};
    int widest_brick{2};
    bool render{true};
    bool ascii{false};
    std::optional<std::string> output;
};

Options read_options(int argc, char** argv)
{
    Options opt;
    while (true)
    {
        static struct option options[] = {
            {"ascii", no_argument, nullptr, 'a'},
            {"count-only", no_argument, nullptr, 'c'},
            {"output", required_argument, nullptr, 'o'},
            {"help", no_argument, nullptr, 'h'},
            {0, 0, 0, 0}};
        int index;
        auto c{getopt_long(argc, argv, "aco:h", options, &index)};
        if (c == -1)
            break;
        switch (c)
        {
        case 0:
            break;
        case 'a':
            opt.ascii = true;
            break;
        case 'c':
            opt.render = false;
            break;
        case 'o':
            opt.output = optarg;
            break;
        case 'h':
            std::cerr << info << std::endl;
            [[fallthrough]];
        default:
            std::cerr << usage << std::endl;
            exit(c == 'h' ? 0 : 1);
        }
    }

    if (optind < argc) opt.n_rows = std::atoi(argv[optind++]);
    if (optind < argc) opt.n_bricks = std::atoi(argv[optind++]);
    if (optind < argc) opt.widest_brick = std::atoi(argv[optind++]);;
    return opt;
}

int main(int argc, char* argv[])
{
    auto const opt{read_options(argc, argv)};

    if (!opt.render && opt.n_rows == 2)
    {
        // Use the fast counting algorithm if 2 courses of 2 bricks.
        std::cout << num_brickworks(opt.n_rows, opt.n_bricks, opt.widest_brick) << std::endl;
        return 0;
    }
    auto walls{generate(opt.n_rows, opt.n_bricks, opt.widest_brick)};
    std::cout << walls.size() << std::endl;
    if (!opt.render)
        return 0;

    if (opt.ascii)
    {
        if (opt.output)
        {
            std::ofstream os{*opt.output};
            ascii_walls(os, walls, 8);
        }
        else
            ascii_walls(std::cout, walls, 8);
    }
    else
        svg_walls((opt.output ? *opt.output : "brickwork") + ".svg", 300, walls, 8);
    return 0;
}
