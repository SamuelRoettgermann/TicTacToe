#pragma once
#include <cstdint>
#include <ostream>

/**
 * A chess game
 */
struct Chess {
    /**
     * Representation of each character, useful for a CLI based output
     */
    static constexpr char
            WHITE_KING = 'K',
            WHITE_QUEEN = 'Q',
            WHITE_ROOK = 'R',
            WHITE_BISHOP = 'B',
            WHITE_KNIGHT = 'N',
            WHITE_PAWN = 'P',
            BLACK_KING = 'k',
            BLACK_QUEEN = 'q',
            BLACK_ROOK = 'r',
            BLACK_BISHOP = 'b',
            BLACK_KNIGHT = 'n',
            BLACK_PAWN = 'p',
            EMPTY_SPACE = '-';

    /**
     * The possible results after trying to execute a move.
     */
    enum class MoveResult {
        ONGOING,
        END_DRAW,
        END_BLACK_WIN,
        END_WHITE_WIN,
        INVALID_MOVE
    };

    /**
     * Represents a chess game-state as 12 separate 64-bit bitboards, 6 for each team for all the pieces.
     * Then it has 2 additional bitboards containing all pieces of the respective team.
     * Additionally, the game-state stores a bitboard of all pieces.
     * On top of that, one bitboard is used to store which pieces have moved, used for castling rights.
     *
     * The bitboards are laid out like this (the numbers represent the bit index, lowermost bit has smallest idx):
     *
     *   56 | 57 | 58 | 59 | 60 | 61 | 62 | 63
     *   -------------------------------------
     *   48 | 49 | 50 | 51 | 52 | 53 | 54 | 55
     *   -------------------------------------
     *   40 | 41 | 42 | 43 | 44 | 45 | 46 | 47
     *   -------------------------------------
     *   32 | 33 | 34 | 35 | 36 | 37 | 38 | 39
     *   -------------------------------------
     *   24 | 25 | 26 | 27 | 28 | 29 | 30 | 31
     *   -------------------------------------
     *   16 | 17 | 18 | 19 | 20 | 21 | 22 | 23
     *   -------------------------------------
     *   08 | 09 | 10 | 11 | 12 | 13 | 14 | 15
     *   -------------------------------------
     *   00 | 01 | 02 | 03 | 04 | 05 | 06 | 07
     *
     * to mimic the layout of the chess notation (a-h, 1-8)
     */
    struct GameState {
        uint64_t
                white_king,
                white_queen,
                white_rooks,
                white_bishops,
                white_knights,
                white_pawns,
                black_king,
                black_queen,
                black_rooks,
                black_bishops,
                black_knights,
                black_pawns,
                white_pieces,
                black_pieces,
                all_pieces,
                moved_pieces;

        bool is_white_turn = true;
    };

public:
    /// Places a piece at the x, y coordinate, where x is in [0,7] (discrete) and maps into [a-h]
    /// and y is also in [0,7] (discrete) and maps into [1-8] (discrete)
    MoveResult move_piece(int from_row, int from_col, int to_row, int to_col);

    /// Returns true iff white is to move
    [[nodiscard]] bool is_white_turn() const;

    /// Returns true iff black is to move
    [[nodiscard]] bool is_black_turn() const;

    /// Returns a number describing how many moves have been played so far
    [[nodiscard]] unsigned moves_played() const;

    /// Returns whether the game is over
    [[nodiscard]] bool game_over() const;

    /// Returns a textual representation of the current game state
    [[nodiscard]] std::string to_string() const;

    /// Returns the current game-state. Useful for automation.
    [[nodiscard]] const GameState &state() const;

    /// Resets the board to the starting position
    void reset();

    /// Returns true iff current player is in check
    [[nodiscard]] bool in_check() const;

    /// Overload to print the current game-state
    friend std::ostream &operator<<(std::ostream &o, const Chess &game) {
        o << game.to_string();
        return o;
    }

private:
    static std::string result_to_str(const MoveResult res) {
        switch (res) {
            case MoveResult::ONGOING:
                return "Ongoing";
            case MoveResult::END_DRAW:
                return "Draw";
            case MoveResult::END_BLACK_WIN:
                return "Black Wins";
            case MoveResult::END_WHITE_WIN:
                return "White Wins";
            case MoveResult::INVALID_MOVE:
                return "Illegal move";
            default:
                return "Invalid state";
        }
    }

    friend std::ostream &operator<<(std::ostream &o, const MoveResult res) {
        o << result_to_str(res);
        return o;
    }

private:
    GameState gamestate;
    unsigned move_count;

};
