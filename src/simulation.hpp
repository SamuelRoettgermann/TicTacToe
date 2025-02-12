#pragma once

#include <iostream>
#include <cstdint>
#include <array>
#include <utility>

using std::uint16_t;

namespace sim {
    enum class Winner {
        DRAW,
        WHITE,
        BLACK
    };

    inline std::ostream &operator<<(std::ostream &o, Winner winner) {
        if (winner == Winner::DRAW)
            o << "Draw";

        if (winner == Winner::WHITE)
            o << "Win White";

        if (winner == Winner::BLACK)
            o << "Win Black";

        return o;
    }

    struct resultdata {
        Winner winner;
        unsigned moves_played;
        uint16_t position_white, position_black;
    };

    inline std::ostream &operator<<(std::ostream &o, const resultdata &res) {
        o << res.winner << " in " << res.moves_played << " moves";

        return o;
    }

    struct Strategy {
        using position = std::pair<unsigned, unsigned>;
        static constexpr position NOT_IMPLEMENTED{-1, -1};

        enum class piece {
            NOTHING,
            WHITE,
            BLACK
        };

        virtual ~Strategy() = default;

        virtual position evaluate(uint16_t bitboard_white, uint16_t bitboard_black);

        virtual position evaluate(std::array<piece, 9>);

        [[nodiscard]] virtual bool hint_bitboard() const;
    };

    resultdata simulate_game(Strategy &strat1, Strategy &strat2);
}
