#include "draw.hh"
#include "row.hh"

#include "simple_svg_1.0.0.hpp"

#include <optional>

namespace fs = std::filesystem;

auto constexpr height_unit {18}; // Rendered length of a brick of length 1.
auto constexpr length_unit {20}; // Rendered height of a brick.
auto constexpr gap {2};          // Size of the mortar gap between bricks.

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
    while (x < max_width)
    {
        for (auto p : row.pattern())
        {
            draw_brick(st_svg, x, y, p);
            x += p*length_unit + gap;
        }
    }
    return st_svg;
}

void svg_wall(fs::path const& file, int const wall_width, Row const& base,
              std::vector<Row> const& bw_rows, int n_reps)
{
    auto const rows_per_wall = 2*n_reps;
    // The total number of rows includes a separator row between each wall.
    auto const total_rows {(rows_per_wall + 1)*bw_rows.size() - 1};
    auto const row_spacing = height_unit + gap; // Distance from one row to the next.
    auto const height {total_rows*row_spacing};
    svg::Document st_svg(file, svg::Layout(svg::Dimensions(wall_width, height),
                                           svg::Layout::TopLeft));
    st_svg << svg::Rectangle(svg::Point(0, 0), wall_width, height, mortar_color);

    auto y {0};
    for (auto const& row : bw_rows)
    {
        for (int i = 0; i < n_reps; ++i)
        {
            draw_row(st_svg, row, y, wall_width);
            y += height_unit + gap;
            draw_row(st_svg, base, y, wall_width);
            y += height_unit + gap;
        }
        y += height_unit + gap;
    }
    st_svg.save();
}
