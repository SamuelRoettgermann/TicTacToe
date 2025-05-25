#include <iostream>

#include "simulation.hpp"
#include "strategies.hpp"
#include "games/Chess/game.h"

namespace strat = sim::strategies;

sim::resultdata sim_random_game() {
    static strat::RandomStrategy s1{}, s2{};
    return simulate_game(s1, s2);
}


int main() {
    Chess game{};
    std::cout << game;
    std::cout << std::endl;
    game.reset();
    std::cout << game << std::endl;
    std::cout << game.move_piece(0, 3, 3, 3) << std::endl << std::endl;
    std::cout << game << std::endl;
    std::cout << game.move_piece(7, 1, 5, 2) << std::endl << std::endl;
    std::cout << game << std::endl;
    std::cout << game.move_piece(0, 5, 2, 7) << std::endl << std::endl;
    std::cout << game << std::endl;
}


// int main() {
//     strat::RandomStrategy s1{}, s2{};
//
//     const int N = 100000;
//
//     int wins_white{}, wins_black{}, draws{};
//     unsigned long moves{};
//     for (int i = 0; i < N; i++) {
//         auto data = sim_random_game();
//         switch (data.winner) {
//             case sim::Winner::WHITE:
//                 wins_white++;
//                 break;
//             case sim::Winner::BLACK:
//                 wins_black++;
//                 break;
//             case sim::Winner::DRAW:
//                 draws++;
//                 break;
//         }
//
//         moves += data.moves_played;
//     }
//
//     std::cout << "White: " << wins_white << ", black: " << wins_black << ", draws: " << draws <<
//             ", moves: " << moves << ", avg moves: " << (static_cast<double>(moves) / N) << std::endl;
// }
