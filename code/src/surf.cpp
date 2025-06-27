#include <iostream>
#include "include/game.h"

int main() {
    Game game;
    try {
        game.run();
    } catch (const std::exception& e) {
        Utils::clear();
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        Utils::clear();
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
    Utils::clear();
    return 0;
}
