#ifndef DRAW_HH
#define DRAW_HH

#include "wall.hh"

#include <string>
#include <vector>

void svg_walls(std::string const& file, int const width, std::vector<Wall> const& walls,
               int n_rows);

#endif
