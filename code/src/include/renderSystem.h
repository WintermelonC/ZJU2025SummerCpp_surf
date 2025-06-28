#pragma once

#include <SFML/Graphics.hpp>
#include "entityManager.h"

class RenderSystem {
public:
    void render(sf::RenderWindow& window);

private:
    EntityManager m_entityManager;  // 实体管理器，用于获取实体的精灵
};
