# Brickwork
Generate interlocked brick patterns.

# Usage
    brickwork [rows] [bricks] [widest]

Unspecified argements are set to 2.

The number of patterns generated is printed to standard output. Walls are rendered in "brickwork.svg".

# Example
To generate the 9 walls with alternating rows made with a repeating pattern of two bricks that may be 1, 2, or 3 units wide, execute

    brickwork 2 2 3

Then "brickwork.svg" contains

![Generated walls](brickwork.svg)

# Details
The criterion for picking rows for the walls is that the vertial gaps between bricks should never align with gaps in adjacent rows. We also require that the repeated pattern has the same number of bricks and the same total length in all rows. If rows had patterns of different lengths, they would slowly get out of sync because they would have different total gap widths per unit length.

The gray brick at the beginnig of some rows is an integer-width offset added before the pattern starts. The offsets of adjacent rows must be different gaps from aligning. The offset is restricted to be narrower that the first brick in the first row.

# Building
    mkdir build
    meson build/
    ninja test -C build/
