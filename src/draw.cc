#include "draw.hh"

#include "simple_svg_1.0.0.hpp"

#include <optional>

auto constexpr height_unit {18}; // Rendered length of a brick of length 1.
auto constexpr length_unit {20}; // Rendered height of a brick.
auto constexpr gap {2};          // Size of the mortar gap between bricks.
auto constexpr row_height {height_unit + gap};

// The color of the optional initial brick.
auto const offset_color {svg::Color(80, 80, 80)};
// The color of the mortar gap.
auto const mortar_color {svg::Color(128, 128, 128)};

/// @return The color a brick from bright red (darkness = 0) to dark gray (darkness = 10).
svg::Color brick_red(int darkness)
{
    return svg::Color(200 - 20*darkness, 20, 20);
}

/// Draw a single brick.
svg::Document& draw_brick(svg::Document& st_svg, int x, int y, int length,
                          std::optional<svg::Color> color = std::nullopt)
{
    return st_svg << svg::Rectangle(svg::Point(x, y), length*length_unit, height_unit,
                                    color ? *color : brick_red(length));
}

/// Draw a repeating row of bricks.
svg::Document& draw_row(svg::Document& st_svg, Row const& row, int y, int max_width)
{
    auto x {0};
    if (row.offset() != 0)
    {
        draw_brick(st_svg, x, y, row.offset(), offset_color);
        x += row.offset()*length_unit + gap;
    }
    // Add 1 to avoid anti-aliasing artifacts at the edge.
    while (x < max_width + 1)
    {
        for (auto p : row.pattern())
        {
            draw_brick(st_svg, x, y, p);
            x += p*length_unit + gap;
        }
    }
    return st_svg;
}

svg::Document& draw_wall(svg::Document& st_svg, Wall const& rows,
                         int y, int width, int n_rows)
{
    for (int i {0}; i < n_rows; )
    {
        for (auto const& row : rows)
        {
            draw_row(st_svg, row, y, width);
            y += row_height;
            if (++i >= n_rows)
                break;
        }
    }
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
               int const width, std::vector<Wall> const& walls, int n_rows)
{
    // The total number of rows includes a separator row between each wall.
    auto const total_rows {walls.empty() ? 0 : (n_rows + 1)*walls.size() - 1};
    auto st_svg {svg_stream(file, width, total_rows*row_height)};
    for (int y {0}; auto const& wall : walls)
    {
        draw_wall(st_svg, wall, y, width, n_rows);
        y += (n_rows + 1)*row_height;
    }
    st_svg.save();
}
