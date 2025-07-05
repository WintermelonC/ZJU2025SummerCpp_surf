#pragma once

#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../views/gameView.h"
#include "../viewModels/spriteViewModel.h"
#include "../viewModels/gameViewModel.h"
#include "../viewModels/fontViewModel.h"
#include "../viewModels/textureViewModel.h"
#include "../viewModels/obstacleItemViewModel.h"
#include "../viewModels/playerViewModel.h"

class Game {
public:
    Game();

    bool initialize();
    void run();

private:
    GameView m_gameView;
    std::shared_ptr<FontViewModel> m_fontViewModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    std::shared_ptr<TextureViewModel> m_textureViewModel;
    std::shared_ptr<GameViewModel> m_gameViewModel;
    std::shared_ptr<ObstacleItemViewModel> m_ObstacleItemViewModel;
    std::shared_ptr<PlayerViewModel> m_playerViewModel;
};