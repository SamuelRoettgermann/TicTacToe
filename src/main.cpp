#include <iostream>
#include <string>

#include "game.hpp"


int main() {
    // Very brief example of how it can be used
    TicTacToe game;
    game.set_piece(0, 1);
    game.set_piece(1, 1);
    game.set_piece(0, 0);
    game.set_piece(2, 1);
    game.set_piece(0, 2);
    std::cout << game;
}