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

#ifndef DRAW_HH
#define DRAW_HH

#include "wall.hh"

#include <iosfwd>
#include <string>
#include <vector>

/// Render an SVG image of the walls to file.
void svg_walls(std::string const& file, int const width, std::vector<Wall> const& walls,
               int n_courses);

/// Send an ASCII rendering of the wall to the stream.
std::ostream& ascii_walls(std::ostream& os, std::vector<Wall> const& walls, int n_courses);

#endif
