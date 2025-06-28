#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite water = m_entityManager.getSprite(EntityType::water, Textures::water);

    window.draw(water);  // 绘制水的精灵

    // 显示渲染结果
    window.display();
}
