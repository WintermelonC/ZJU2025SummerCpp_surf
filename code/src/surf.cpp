#include <iostream>
#include "assetManager.h"
#include "game.h"

int main() {
    AssetManager::loadAssets();
    EntityManager::loadSprites();
    Game game;
    try {
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
