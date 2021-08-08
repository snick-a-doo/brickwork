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

#endif // BRICKWORK_HH
