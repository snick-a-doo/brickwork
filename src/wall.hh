#ifndef ROW_HH
#define ROW_HH

#include <iosfwd>
#include <string>
#include <vector>

/// A row of a brick wall described by an offset an a repeated pattern of lengths.
class Row
{
public:
    /// Construct a row from a one-time offset and a vector of lengths to repeat.
    Row(int offset, std::vector<int> const& lengths)
        : m_offset {offset}, m_pattern{lengths} {}
    /// @return The offset.
    int offset() const { return m_offset; }
    /// @return A vector of the repeated lengths.
    std::vector<int> const& pattern() const {return m_pattern; }
    /// The total length of the repeated pattern.
    int period() const;

    /// @return A text representation of the row. A brick is shown as a number of
    /// spaces equal to its length separated by |. The total length is 80 characters.
    operator std::string() const;
    /// Compare two rows.
    auto operator <=>(Row const& row) const = default;

private:
    /// An initial brick of length m_offset.
    int m_offset;
    /// The repeated bricks.
    std::vector<int> m_pattern;
};

/// Send the string representation to the stream.
std::ostream& operator <<(std::ostream& os, Row const& row);

using Wall = std::vector<Row>;

#endif // ROW_HH
