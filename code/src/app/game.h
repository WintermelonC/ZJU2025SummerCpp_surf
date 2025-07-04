#pragma once

#include <SFML/Graphics.hpp>
#include "../views/gameView.h"
#include "../viewModels/spriteViewModel.h"
#include "../viewModels/gameViewModel.h"
#include "../viewModels/fontViewModel.h"
#include "../common/config.h"

class Game {
public:
    Game();

    bool initialize();
    void run();

private:
    void handleEvents(sf::RenderWindow& window);

private:
    GameView m_gameView;
    std::shared_ptr<FontViewModel> m_fontViewModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    std::shared_ptr<GameViewModel> m_gameViewModel;
};