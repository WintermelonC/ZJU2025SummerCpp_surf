#include "gameView.h"

GameView::GameView() 
    : m_map(nullptr),
      m_water(nullptr) {}

bool GameView::initialize(unsigned int width, unsigned int height, const std::string& title) {
    // 创建 SFML 渲染窗口
    m_window = sf::RenderWindow(sf::VideoMode({width, height}), title);
    m_view = sf::View({1280, 720}, {static_cast<float>(width), static_cast<float>(height)});
    m_window.setView(m_view);

    // 启用垂直同步
    m_window.setVerticalSyncEnabled(true);

    return true;
}

void GameView::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // 处理窗口大小调整事件
            m_view.setSize(sf::Vector2f(resized->size));
            m_window.setView(m_view);
        }
    }
}

void GameView::render() {
    m_window.clear(sf::Color(0, 192, 222));
    if (m_map != nullptr) {
        for (size_t i = 0; i < m_map->getSize(); i++) {
            const auto& entity = m_map->getEntity(i);
            switch (entity.type) {
                case EntityType::water: {
                    m_water->get()->setPosition(entity.position);
                    m_water->get()->setOrigin(entity.origin);
                    m_water->get()->setScale(entity.scale);
                    m_window.draw(*m_water->get());
                    break;
                } case EntityType::player: {
                    m_player->get()->setOrigin(entity.origin);
                    m_player->get()->setPosition(entity.position);
                    m_player->get()->setScale(entity.scale);
                    m_player->get()->setTexture(entity.texture);
                    m_window.draw(*m_player->get());

                #ifdef DEBUG
                    m_window.draw(entity.collisionBox);
                #endif // DEBUG
                    break;
                } default: {
                    std::cerr << "Unknown entity type: " << static_cast<int>(entity.type) << std::endl;
                    break;
                }
            }
        }
    }
    m_window.display();
}