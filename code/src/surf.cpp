#include <iostream>
#include "app/game.h"

int main() {
    Game game;

    if (!game.initialize()) {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    game.run();
}
