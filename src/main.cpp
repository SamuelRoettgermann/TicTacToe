#include <iostream>

#include "simulation.hpp"
#include "strategies.hpp"

namespace strat = sim::strategies;

sim::resultdata sim_random_game() {
    static strat::RandomStrategy s1{}, s2{};
    return simulate_game(s1, s2);
}


int main() {
    strat::RandomStrategy s1{}, s2{};

    const int N = 10000000;

    int wins_white{}, wins_black{}, draws{};
    unsigned long moves{};
    for (int i = 0; i < N; i++) {
        auto data = sim_random_game();
        switch (data.winner) {
            case sim::Winner::WHITE:
                wins_white++;
                break;
            case sim::Winner::BLACK:
                wins_black++;
                break;
            case sim::Winner::DRAW:
                draws++;
                break;
        }

        moves += data.moves_played;
    }

    std::cout << "White: " << wins_white << ", black: " << wins_black << ", draws: " << draws <<
            ", moves: " << moves << ", avg moves: " << (static_cast<double>(moves) / N) << std::endl;
}
