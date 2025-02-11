Simple implementation of TicTacToe using a bitboard per player to store the pieces

API is as follows:

```
/// Places a piece at the x, y coordinates and returns a result instance, indicating the current status
result set_piece(unsigned const x, unsigned const y);

/// Returns true iff white is to move
bool is_white_turn() const;

/// Returns true iff black is to move
bool is_black_turn() const;

/// Returns a number describing how many moves have been played so far
unsigned moves_played() const;

/// Returns a number describing how many moves still need to be played
unsigned moves_left() const;

/// Returns a textual representation of the current game state 
std::string to_string() const;
```