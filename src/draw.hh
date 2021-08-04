#ifndef DRAW_HH
#define DRAW_HH

#include "row.hh"

#include <filesystem>
#include <vector>

void svg_wall(std::filesystem::path const& file, int const width,
              Row const& base, std::vector<Row> const& rows, int n_reps);

#endif
