#include <string>
#include <algorithm>
#include <format>
#include <vector>
#include <cstdint>
#include <immintrin.h>

#include "game.hpp"

using std::uint16_t;

bool TicTacToe::is_white_turn() const {
    return white_turn;
}

bool TicTacToe::is_black_turn() const {
    return not white_turn;
}

unsigned TicTacToe::moves_played() const {
    return std::min(move_count, 9u);
}

unsigned TicTacToe::moves_left() const {
    return std::max(9 - move_count, 0u);
}

bool TicTacToe::game_over() const {
    return last_state != result::ONGOING && last_state != result::INVALID_MOVE;
}

static inline bool is_win(const uint16_t pieces) {
    // Generated with mask_generation.py
    static const __m128i win_masks = _mm_set_epi16(0x07, 0x49, 0x38, 0x92, 0x1c0, 0x124, 0b001010100, 0b100010001);

    __m128i eval = _mm_set1_epi16(static_cast<int16_t>(pieces));
    eval = _mm_and_si128(eval, win_masks);
    eval = _mm_cmpeq_epi16(eval, win_masks);
    return !_mm_test_all_zeros(eval, eval);
}

static inline uint16_t generate_insert_mask(const unsigned x, const unsigned y) {
    return 1 << (x + y * 3);
}

TicTacToe::result TicTacToe::set_piece(unsigned const x, unsigned const y) {
    if (x > 2 || y > 2) {
        // if coords are not in range [0, 2]
        return last_state = result::INVALID_MOVE;
    }

    const uint16_t insert_mask = generate_insert_mask(x, y);
    // validate that the field isn't already taken
    if (insert_mask & (white_board | black_board)) {
        return last_state = result::INVALID_MOVE;
    }

    uint16_t &current_board = white_turn ? white_board : black_board;
    current_board |= insert_mask;
    move_count += 1;

    if (is_win(current_board)) {
        last_state = white_turn ? result::END_WHITE_WIN : result::END_BLACK_WIN;
        goto end_change_player;
    }

    last_state = moves_left() ? result::ONGOING : result::END_DRAW;
end_change_player:
    white_turn = not white_turn;

    return last_state;
}

std::string TicTacToe::to_string() const {
    std::vector<char> pieces;
    pieces.reserve(9);
    for (unsigned i = 0u, mask = 1u; i < 9u; i++, mask <<= 1) {
        char cur = EMPTY_SYMBOL;
        if (white_board & mask)
            cur = WHITE_SYMBOL;
        else if (black_board & mask)
            cur = BLACK_SYMBOL;

        pieces.push_back(cur);
    }

    return std::format(
        "{}|{}|{}\n"
        "-----\n"
        "{}|{}|{}\n"
        "-----\n"
        "{}|{}|{}\n"
        "\n"
        "Moves: {}/9\n"
        "Current state: {}\n",
        pieces[0], pieces[1], pieces[2],
        pieces[3], pieces[4], pieces[5],
        pieces[6], pieces[7], pieces[8],
        moves_played(),
        result_to_str(last_state)
    );
}

TicTacToe::gamestate TicTacToe::state() const {
    return {white_board, black_board};
}
