#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite& water = EntityManager::getSprite(EntityType::water);
    sf::Sprite& player = EntityManager::getSprite(EntityType::player);

    window.draw(water);  // 绘制水
    window.draw(player); // 绘制玩家

    // 显示渲染结果
    window.display();
}
