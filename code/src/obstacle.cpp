#include "include/obstacle.h"
#include "include/game.h"
#include "iostream"

/**
 * @brief 更新障碍物的位置
 * @param dt 帧间隔时间，用于实现帧率无关的移动
 * @param playerVelocity 玩家的完整速度向量
 * * 该函数的移动原理与背景移动类似，都是根据玩家的速度进行反向运动，
 * 以此来创造出玩家在移动而障碍物静止在世界中的错觉。
 */
void Obstacle::update(float dt, const sf::Vector2f& playerVelocity){
    float moveX = -playerVelocity.x * 0.1;
    float moveY = -playerVelocity.y * 0.1;

    m_sprite.move(sf::Vector2f(moveX, moveY));
}