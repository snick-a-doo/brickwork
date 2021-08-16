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

#include "draw.hh"

#include "simple_svg_1.0.0.hpp"

#include <optional>

auto constexpr height_unit{9}; // Rendered length of a brick of length 1.
auto constexpr length_unit{10}; // Rendered height of a brick.
auto constexpr gap{1};          // Size of the mortar gap between bricks.
auto constexpr row_height{height_unit + gap};

// The color of the optional initial brick.
auto const offset_color{svg::Color(80, 80, 80)};
// The color of the mortar gap.
auto const mortar_color{svg::Color(128, 128, 128)};

/// @return The color a brick from bright red (darkness = 0) to dark gray (darkness = 10).
svg::Color brick_red(int darkness)
{
    return svg::Color(170 - 20*darkness, 50, 30);
}

/// Draw a single brick.
svg::Document& draw_brick(svg::Document& st_svg, int x, int y, int length,
                          std::optional<svg::Color> color = std::nullopt)
{
    return st_svg << svg::Rectangle(svg::Point(x, y), length*length_unit - gap,
                                    height_unit, color ? *color : brick_red(length));
}

/// Draw a repeating row of bricks.
svg::Document& draw_row(svg::Document& st_svg, Row const& row, int y, int max_width)
{
    auto x{0};
    if (row.offset() != 0)
    {
        draw_brick(st_svg, x, y, row.offset(), offset_color);
        x += row.offset()*length_unit;
    }
    // Add 1 to avoid anti-aliasing artifacts at the edge.
    while (x < max_width + 1)
    {
        for (auto p : row.pattern())
        {
            draw_brick(st_svg, x, y, p);
            x += p*length_unit;
        }
    }
    return st_svg;
}

svg::Document& draw_wall(svg::Document& st_svg, Wall const& courses,
                         int y, int width, int n_courses)
{
    // Draw the courses backwards so the base is a the bottom (largest y coordinate).
    for (auto i{n_courses}; i-- > 0; y += row_height)
        draw_row(st_svg, courses[i % courses.size()], y, width);
    return st_svg;
}

svg::Document svg_stream(std::string const& file, int width, int height)
{
    svg::Document st_svg(file, svg::Layout(svg::Dimensions(width, height),
                                           svg::Layout::TopLeft));
    // Add 1 to width and height to avoid anti-aliasing artifacts.
    return st_svg << svg::Rectangle(svg::Point(0, 0), width + 1, height + 1, mortar_color);
}

void svg_walls(std::string const& file,
               int const width, std::vector<Wall> const& walls, int n_courses)
{
    // The total number of rows includes a separator row between each wall.
    auto const total_rows {walls.empty() ? 0 : (n_courses + 1)*walls.size() - 1};
    auto st_svg{svg_stream(file, width, total_rows*row_height)};
    for (int y{0}; auto const& wall : walls)
    {
        draw_wall(st_svg, wall, y, width, n_courses);
        y += (n_courses + 1)*row_height;
    }
    st_svg.save();
}

std::ostream& ascii_walls(std::ostream& os, std::vector<Wall> const& walls, int n_courses)
{
    for (auto const& wall : walls)
    {
        for (int i{n_courses}; i-- > 0;)
            os << wall[i % wall.size()] << '\n';
        os << '\n';
    }
    return os;
}
