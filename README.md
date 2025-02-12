Old README:

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

Please note that x describes the column and y the row when looking at the printed version.

///////////////////////////////////////////////////////////////////////////////////////////

New README:

This project used to be just a TicTacToe game, but the idea shifted to a simulation software for TicTacToe.

The idea would be to eventually extend the program to support multiple games, or even that one can add their own games and Strategy signatures, but that would require an enormous re-write, and right now I want to keep the focus on the simulation part, and try to think of some "cool" analysis ideas.

Right now the API is effectively just `sim::resultdata sim::simulate_game(sim::Strategy&, sim::Strategy&)`, where `sim::resultdata` is just a small data-struct and `Strategy` is a base class that allows the user to override either or both of the evaluate functions `virtual sim::Strategy::position evaluate(uint16_t bitboard_white, uint16_t bitboard_black)` and `virtual sim::Strategy::position evaluate(std::array<piece, 9>)`. The simulation software allows, but doesn't force, the user to override `virtual bool hint_bitboard() const` if they want to explicitly state which algorithm they want to use, but the simulation will automatically fall back to the second implementation, if it detects that the user didn't override it.
