#include <iostream>
#include <cstdint>
#include <array>
#include <utility>
#include <format>

#include "games/TicTacToe/game.hpp"
#include "simulation.hpp"

using std::uint16_t;

namespace sim {

    Strategy::position Strategy::evaluate(uint16_t bitboard_white, uint16_t bitboard_black) {
        (void) bitboard_white;
        (void) bitboard_black;
        return NOT_IMPLEMENTED;
    };

    Strategy::position Strategy::evaluate(std::array<piece, 9>) {
        return NOT_IMPLEMENTED;
    }

    [[nodiscard]] bool Strategy::hint_bitboard() const {
        return true;
    }

    static inline std::array<Strategy::piece, 9> bits_to_pieces(uint16_t bitboard_white, uint16_t bitboard_black) {
        std::array<Strategy::piece, 9> pieces{};
        for (int i = 0, mask = 1; i < 9; i++, mask <<= 1) {
            if (bitboard_white & mask)
                pieces[i] = Strategy::piece::WHITE;
            else if (bitboard_black & mask)
                pieces[i] = Strategy::piece::BLACK;
            else
                pieces[i] = Strategy::piece::NOTHING;
        }

        return pieces;
    }

    resultdata simulate_game(Strategy &strat1, Strategy &strat2) {
        const std::array<bool, 2> use_bitboard{strat1.hint_bitboard(), strat2.hint_bitboard()};

        TicTacToe game{};
        auto last_result = TicTacToe::result::ONGOING;
        for (int current_player = 0; !game.game_over(); current_player = !current_player) {
            Strategy &current_strat = current_player == 0 ? strat1 : strat2;

            // TODO: Refactor this function by extracting (common) functionality out of it
            bool first_strat = false;

            if (use_bitboard[current_player]) {
            try_strat1:
                const auto [board_white, board_black] = game.state();
                Strategy::position suggestion = current_strat.evaluate(board_white, board_black);
                if (suggestion == Strategy::NOT_IMPLEMENTED) {
                    goto try_strat2;
                }

                auto [x, y] = suggestion;
                last_result = game.set_piece(x, y);
                continue;
            }


            first_strat = true;

        try_strat2:
            const auto [board_white, board_black] = game.state();
            auto pieces = bits_to_pieces(board_white, board_black);
            Strategy::position suggestion = current_strat.evaluate(pieces);
            if (suggestion == Strategy::NOT_IMPLEMENTED) {
                if (!first_strat)
                    throw std::invalid_argument(std::format("Strategy for {} doesn't provide an evaluate function",
                                                            current_player == 0 ? "white" : "black"));

                goto try_strat1;
            }

            auto [x, y] = suggestion;
            last_result = game.set_piece(x, y);
        }

        auto [pos_white, pos_black] = game.state();
        resultdata result = {
            .winner = Winner::DRAW,  // temporary, will be overwritten
            .moves_played = game.moves_played(),
            .position_white = pos_white,
            .position_black = pos_black
        };

        switch (last_result) {
            case TicTacToe::result::END_DRAW:
                result.winner = Winner::DRAW;
                break;
            case TicTacToe::result::END_WHITE_WIN:
                result.winner = Winner::WHITE;
                break;
            case TicTacToe::result::END_BLACK_WIN:
                result.winner = Winner::BLACK;
                break;
            default:
                throw std::invalid_argument("Invalid last result state, last game should not have ended");
        }

        return result;
    }
}