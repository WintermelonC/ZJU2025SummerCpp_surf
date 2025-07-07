#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

class ScoreManager {
public:
    static ScoreManager& getInstance();
    
    // 保存最高分到JSON文件
    void saveHighScore(float score);
    
    // 从JSON文件加载最高分
    float loadHighScore();
    
    // 检查并更新最高分
    bool updateHighScoreIfNeeded(float currentScore);
    
    // 获取当前最高分
    float getHighScore() const { return m_highScore; }

private:
    ScoreManager() = default;
    ~ScoreManager() = default;
    ScoreManager(const ScoreManager&) = delete;
    ScoreManager& operator=(const ScoreManager&) = delete;
    
    void loadHighScoreFromFile();
    void saveHighScoreToFile();
    void ensureDirectoryExists(const std::string& filePath);
    std::string createJsonString(float score);
    float parseJsonString(const std::string& jsonContent);
    
    static const std::string SCORE_FILE_PATH;
    float m_highScore = 0.0f;
    bool m_isLoaded = false;
};
