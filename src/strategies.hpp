#pragma once

#include <cstdint>
#include <random>
#include <utility>

#include "simulation.hpp"

using std::uint16_t;


namespace sim::strategies {
    struct RandomStrategy final : sim::Strategy {
        std::random_device rd; // a seed source for the random number engine
        std::mt19937 gen; // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib;

        RandomStrategy() : rd{}, gen{rd()}, distrib{0, 2} {
        }


        position evaluate(uint16_t, uint16_t) override;
    };
}
