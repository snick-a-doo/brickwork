#ifndef BRICKWORK_HH
#define BRICKWORK_HH

#include "wall.hh"

#include <vector>

/// @return True if the two rows don't have any gaps that line up.
bool is_brickwork(Row const& lower, Row const& upper);

// Return a vector of all brick rows that form a brickwork with the argument by varying
// the offset, and permuting the pattern.
std::vector<Wall> generate(Row const& row1, Row const& row2);

#endif // BRICKWORK_HH
