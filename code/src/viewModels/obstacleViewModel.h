#pragma once

#include <random>
#include <SFML/Graphics.hpp>
#include "../models/obstacleModel.h"

class ObstacleViewModel {
public:
    ObstacleViewModel();

    void update(const float& dt);

private:
    void initialize();

private:
    const float m_spawnInterval = 0.5f;  // 生成间隔时间

    // 障碍物组模式结构
    struct ObstaclePattern {
        std::vector<sf::Vector2f> positions;  // 相对位置
        std::vector<ObstacleType> obstacleTypes;  // 障碍物类型（可选，用于预设）
        float width;  // 模式宽度
        float height;  // 模式高度
    };
    
    std::vector<ObstaclePattern> m_obstaclePatterns;  // 预定义的障碍物组模式
    sf::Clock m_spawnClock;  // 生成时钟
    
    // 随机数生成器
    std::random_device m_rd;
    std::mt19937 m_gen;
    
    // 生成模式权重（单个:小组:大组）
    std::vector<int> m_spawnWeights{1, 2, 3};
};