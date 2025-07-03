#pragma once

#include <SFML/Graphics.hpp>
#include "../views/gameView.h"
#include "../viewModels/spriteViewModel.h"
#include "../viewModels/gameViewModel.h"

class Game {
public:
    Game();

    bool initialize();
    void run();

private:
    GameView m_gameView;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    std::shared_ptr<GameViewModel> m_gameViewModel;
};