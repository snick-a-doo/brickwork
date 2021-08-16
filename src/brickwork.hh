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

#ifndef BRICKWORK_HH
#define BRICKWORK_HH

#include "wall.hh"

#include <vector>

/// @return a vector will all possible brickworks of n_rows rows consisting of a pattern
/// of n_bricks from 1 to widest_brick units wide. The bricks in a pattern do not
/// necessarily have unique widths.
std::vector<Wall> generate(int n_rows, int n_bricks, int widest_brick);

/// @return True if the two rows don't have any gaps that line up.
bool is_brickwork(Row const& lower, Row const& upper);

/// @return Calculated number of brickworks. Currently only implemented for 2 rows and 2
/// bricks. These values are asserted.
int num_brickworks(int n_rows, int n_bricks, int widest_brick);

#endif // BRICKWORK_HH
