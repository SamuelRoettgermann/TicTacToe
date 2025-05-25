#pragma once

#include <string>
#include <cstdint>
#include <iostream>

using std::uint16_t;

class TicTacToe {
    static constexpr char WHITE_SYMBOL = 'O', BLACK_SYMBOL = 'X', EMPTY_SYMBOL = ' ';

public:
    enum class result {
        ONGOING,
        END_DRAW,
        END_BLACK_WIN,
        END_WHITE_WIN,
        INVALID_MOVE
    };

    struct gamestate {
        uint16_t bitboard_white, bitboard_black;
    };

public:
    TicTacToe() : white_board{0}, black_board{0}, move_count{0}, white_turn{true}, last_state{result::ONGOING} {
    }

    /// Places a piece at the x, y coordinates and returns a result instance, indicating the current status
    result set_piece(unsigned x, unsigned y);

    /// Returns true iff white is to move
    [[nodiscard]] bool is_white_turn() const;

    /// Returns true iff black is to move
    [[nodiscard]] bool is_black_turn() const;

    /// Returns a number describing how many moves have been played so far
    [[nodiscard]] unsigned moves_played() const;

    /// Returns a number describing how many moves still need to be played
    [[nodiscard]] unsigned moves_left() const;

    /// Returns whether the game is over 
    [[nodiscard]] bool game_over() const;

    /// Returns a textual representation of the current game state 
    [[nodiscard]] std::string to_string() const;

    /// Returns the current gamestate. Useful for automation. 
    [[nodiscard]] gamestate state() const;

    /// Overload to print the current gamestate
    friend std::ostream &operator<<(std::ostream &o, const TicTacToe &game) {
        o << game.to_string();
        return o;
    }

private:
    static std::string result_to_str(const result res) {
        switch (res) {
            case result::ONGOING:
                return "Ongoing";
            case result::END_DRAW:
                return "Draw";
            case result::END_BLACK_WIN:
                return "Black Wins";
            case result::END_WHITE_WIN:
                return "White Wins";
            case result::INVALID_MOVE:
                return "Illegal move";
            default:
                return "Invalid state";
        }
    }

    friend std::ostream &operator<<(std::ostream &o, const result res) {
        o << result_to_str(res);
        return o;
    }

private:
    uint16_t white_board, black_board;
    unsigned move_count;
    bool white_turn;
    result last_state;
};
