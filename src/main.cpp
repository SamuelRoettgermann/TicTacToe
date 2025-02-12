#include <iostream>
#include <cstdint>

#include "simulation.hpp"
#include "strategies.hpp"

namespace strat = sim::strategies;

int main() {
    strat::RandomStrategy s1{}, s2{};
    std::cout << sim::simulate_game(s1, s2) << "\n";
}
