#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 在这里添加渲染逻辑，例如绘制精灵、形状等

    // 显示渲染结果
    window.display();
}
