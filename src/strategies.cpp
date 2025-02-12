#include <cstdint>
#include <random>
#include <utility>

#include "simulation.hpp"
#include "strategies.hpp"

using std::uint16_t;



namespace sim::strategies {
        Strategy::position RandomStrategy::evaluate(uint16_t, uint16_t) {
            return {distrib(gen), distrib(gen)};
        }
}
