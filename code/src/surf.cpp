#include <iostream>
#include "mvvmGame.h"

int main() {
    MVVMGame game;
    try {
        game.initialize();
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }
    return 0;
}
