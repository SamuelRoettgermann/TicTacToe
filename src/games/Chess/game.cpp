//
// Created by samur on 22/05/2025.
//


#include "game.h"

#include <vector>
#include <bit>
#include <sstream>
#include <cstdlib>

bool Chess::game_over() const {
    return std::popcount(gamestate.white_king | gamestate.black_king) != 2;
}

bool Chess::is_white_turn() const {
    return gamestate.is_white_turn;
}

bool Chess::is_black_turn() const {
    return not gamestate.is_white_turn;
}

unsigned Chess::moves_played() const {
    return move_count;
}

const Chess::GameState &Chess::state() const {
    return gamestate;
}

static void set_if_bit_active(char &piece, const uint64_t mask, const uint64_t board, const char value) {
    if (mask & board) {
        piece = value;
    }
}

std::string Chess::to_string() const {
    std::vector<char> pieces_text;
    pieces_text.reserve(64);

    for (int i = 0; i < 64; i++) {
        const uint64_t current_bit = static_cast<uint64_t>(1) << i;

        // default piece
        char current_piece = '-';

        // set if white piece
        set_if_bit_active(current_piece, current_bit, gamestate.white_king, WHITE_KING);
        set_if_bit_active(current_piece, current_bit, gamestate.white_queen, WHITE_QUEEN);
        set_if_bit_active(current_piece, current_bit, gamestate.white_rooks, WHITE_ROOK);
        set_if_bit_active(current_piece, current_bit, gamestate.white_bishops, WHITE_BISHOP);
        set_if_bit_active(current_piece, current_bit, gamestate.white_knights, WHITE_KNIGHT);
        set_if_bit_active(current_piece, current_bit, gamestate.white_pawns, WHITE_PAWN);

        // set if black piece
        set_if_bit_active(current_piece, current_bit, gamestate.black_king, BLACK_KING);
        set_if_bit_active(current_piece, current_bit, gamestate.black_queen, BLACK_QUEEN);
        set_if_bit_active(current_piece, current_bit, gamestate.black_rooks, BLACK_ROOK);
        set_if_bit_active(current_piece, current_bit, gamestate.black_bishops, BLACK_BISHOP);
        set_if_bit_active(current_piece, current_bit, gamestate.black_knights, BLACK_KNIGHT);
        set_if_bit_active(current_piece, current_bit, gamestate.black_pawns, BLACK_PAWN);

        pieces_text.push_back(current_piece);
    }


    std::stringstream result;
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            result << ' ' << pieces_text[row * 8 + col];
        }

        result << '\n';
    }

    return result.str();
}

constexpr static uint64_t set_bits() {
    return 0;
}

template<typename T>
constexpr static uint64_t set_bits(T n) {
    return static_cast<uint64_t>(1) << n;
}

template<typename Head, typename... Tail>
constexpr static uint64_t set_bits(Head head, Tail... tail) {
    return set_bits(head) | set_bits(tail...);
}

constexpr static unsigned cord_to_bitidx(unsigned x, unsigned y) {
    return x * 8 + y;
}

constexpr static bool is_neighbour(int from_row, int from_col, int to_row, int to_col) {
    return std::abs(to_row - from_row) <= 1 && std::abs(to_col - from_col) <= 1;
}

constexpr static bool is_horizontal(int from_row, int to_row) {
    return from_row == to_row;
}

constexpr static bool is_vertical(int from_col, int to_col) {
    return from_col == to_col;
}

constexpr static bool is_straight(int from_row, int from_col, int to_row, int to_col) {
    return is_horizontal(from_row, to_row) || is_vertical(from_col, to_col);
}

constexpr static bool is_diagonal(int from_row, int from_col, int to_row, int to_col) {
    return std::abs(to_row - from_row) == std::abs(to_col - from_col);
}

constexpr static bool is_L_shape(int from_row, int from_col, int to_row, int to_col) {
    const auto row_diff = std::abs(to_row - from_row);
    const auto col_diff = std::abs(to_col - from_col);

    return (row_diff == 1 && col_diff == 2) || (row_diff == 2 && col_diff == 1);
}

Chess::MoveResult Chess::move_piece(const int from_row, const int from_col, const int to_row, const int to_col) {
    // Check if move is out-of-bounds
    if (from_row < 0 || from_row >= 8
        || from_col < 0 || from_col >= 8
        || to_row < 0 || to_row >= 8
        || to_col < 0 || to_col >= 8) {
        return MoveResult::INVALID_MOVE;
    }

    const unsigned from_idx = cord_to_bitidx(from_row, from_col);
    const unsigned to_idx = cord_to_bitidx(to_row, to_col);

    // Check if move doesn't move
    if (from_idx == to_idx) {
        return MoveResult::INVALID_MOVE;
    }

    const uint64_t from_mask = set_bits(from_idx);
    const uint64_t to_mask = set_bits(to_idx);

    // Check if move tries to move a piece that doesn't exist or doesn't belong to current team
    if ((is_white_turn() && !(from_mask & gamestate.white_pieces))
        || (is_black_turn() && !(from_mask & gamestate.black_pieces))) {
        return MoveResult::INVALID_MOVE;
    }

    // Define aliases
    auto &all_pieces = gamestate.all_pieces;
    auto &team_pieces = is_white_turn() ? gamestate.white_pieces : gamestate.black_pieces;
    auto &opponent_pieces = is_white_turn() ? gamestate.black_pieces : gamestate.white_pieces;

    auto &team_king = is_white_turn() ? gamestate.white_king : gamestate.black_king;
    auto &team_queens = is_white_turn() ? gamestate.white_queen : gamestate.black_queen;
    auto &team_rooks = is_white_turn() ? gamestate.white_rooks : gamestate.black_rooks;
    auto &team_bishops = is_white_turn() ? gamestate.white_bishops : gamestate.black_bishops;
    auto &team_knights = is_white_turn() ? gamestate.white_knights : gamestate.black_knights;
    auto &team_pawns = is_white_turn() ? gamestate.white_pawns : gamestate.black_pawns;

    auto &opponent_king = is_black_turn() ? gamestate.white_king : gamestate.black_king;
    auto &opponent_queens = is_black_turn() ? gamestate.white_queen : gamestate.black_queen;
    auto &opponent_rooks = is_black_turn() ? gamestate.white_rooks : gamestate.black_rooks;
    auto &opponent_bishops = is_black_turn() ? gamestate.white_bishops : gamestate.black_bishops;
    auto &opponent_knights = is_black_turn() ? gamestate.white_knights : gamestate.black_knights;
    auto &opponent_pawns = is_black_turn() ? gamestate.white_pawns : gamestate.black_pawns;


    // TODO: A whole bunch of checks if moves are legal
    const bool neighbour = is_neighbour(from_row, from_col, to_row, to_col);
    const bool straight = is_straight(from_row, from_col, to_row, to_col);
    const bool diag = is_diagonal(from_row, from_col, to_row, to_col);
    const bool l_shape = is_L_shape(from_row, from_col, to_row, to_col);

    // Remove and re-appear a team's piece, i.e. move it
    if (const auto king_remove_mask = team_king & from_mask) {
        // We move the king
        if (not neighbour) {
            return MoveResult::INVALID_MOVE;
        }

        team_king ^= king_remove_mask | to_mask;
    } else if (const auto queen_remove_mask = team_queens & from_mask) {
        // We move a queen
        if (not (straight || diag)) {
            return MoveResult::INVALID_MOVE;
        }

        team_queens ^= queen_remove_mask | to_mask;
    } else if (const auto rook_remove_mask = team_rooks & from_mask) {
        // We move a rook
        if (not straight) {
            return MoveResult::INVALID_MOVE;
        }

        team_rooks ^= rook_remove_mask | to_mask;
    } else if (const auto bishop_remove_mask = team_bishops & from_mask) {
        // We move a bishop
        if (not diag) {
            return MoveResult::INVALID_MOVE;
        }

        team_bishops ^= bishop_remove_mask | to_mask;
    } else if (const auto knight_remove_mask = team_knights & from_mask) {
        // We move a knight
        if (not l_shape) {
            return MoveResult::INVALID_MOVE;
        }

        team_knights ^= knight_remove_mask | to_mask;
    } else if (const auto pawns_remove_mask = team_pawns & from_mask) {
        // We move a pawn
        if (not neighbour) {
            return MoveResult::INVALID_MOVE;
        }

        team_pawns ^= pawns_remove_mask | to_mask;
    }

    // remove an opponent's piece if we landed on one
    opponent_king ^= opponent_king & to_mask;
    opponent_queens ^= opponent_queens & to_mask;
    opponent_rooks ^= opponent_rooks & to_mask;
    opponent_bishops ^= opponent_bishops & to_mask;
    opponent_knights ^= opponent_knights & to_mask;
    opponent_pawns ^= opponent_pawns & to_mask;

    // Adjust the global bitboards
    team_pieces ^= from_mask;
    opponent_pieces ^= to_mask;
    all_pieces ^= from_mask | to_mask;

    move_count += 1;
    gamestate.is_white_turn = not gamestate.is_white_turn;

    return MoveResult::ONGOING;
}


void Chess::reset() {
    // Reset white pieces
    gamestate.white_king = set_bits(4);
    gamestate.white_queen = set_bits(3);
    gamestate.white_rooks = set_bits(0, 7);
    gamestate.white_bishops = set_bits(2, 5);
    gamestate.white_knights = set_bits(1, 6);
    gamestate.white_pawns = set_bits(8, 9, 10, 11, 12, 13, 14, 15);

    // Reset black pieces
    gamestate.black_king = set_bits(60);
    gamestate.black_queen = set_bits(59);
    gamestate.black_rooks = set_bits(56, 63);
    gamestate.black_bishops = set_bits(58, 61);
    gamestate.black_knights = set_bits(57, 62);
    gamestate.black_pawns = set_bits(48, 49, 50, 51, 52, 53, 54, 55);

    // Reset combined bitboards
    gamestate.white_pieces = gamestate.white_king
                             | gamestate.white_queen
                             | gamestate.white_rooks
                             | gamestate.white_bishops
                             | gamestate.white_knights
                             | gamestate.white_pawns;

    gamestate.black_pieces = gamestate.black_king
                             | gamestate.black_queen
                             | gamestate.black_rooks
                             | gamestate.black_bishops
                             | gamestate.black_knights
                             | gamestate.black_pawns;

    gamestate.all_pieces = gamestate.white_pieces | gamestate.black_pieces;

    // Reset general metadata
    gamestate.is_white_turn = true;
}
