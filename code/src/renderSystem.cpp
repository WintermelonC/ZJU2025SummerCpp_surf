#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite waterSprite(AssetManager::getTexture(Textures::water));

    window.draw(waterSprite);  // 渲染水纹理

    // 显示渲染结果
    window.display();
}
