#include "scoreManager.h"
#include <sstream>
#include <filesystem>

const std::string ScoreManager::SCORE_FILE_PATH = "../../surf_data/highscore.json";

ScoreManager& ScoreManager::getInstance() {
    static ScoreManager instance;
    return instance;
}

void ScoreManager::saveHighScore(float score) {
    m_highScore = score;
    saveHighScoreToFile();
}

float ScoreManager::loadHighScore() {
    if (!m_isLoaded) {
        loadHighScoreFromFile();
        m_isLoaded = true;
    }
    return m_highScore;
}

bool ScoreManager::updateHighScoreIfNeeded(float currentScore) {
    if (!m_isLoaded) {
        loadHighScore();
    }
    
    if (currentScore > m_highScore) {
        saveHighScore(currentScore);
        return true;  // 返回true表示更新了最高分
    }
    return false;  // 返回false表示没有更新
}

void ScoreManager::loadHighScoreFromFile() {
    // 确保目录存在
    ensureDirectoryExists(SCORE_FILE_PATH);
    
    std::ifstream file(SCORE_FILE_PATH);
    if (!file.is_open()) {
        // 文件不存在，创建默认分数
        m_highScore = 0.0f;
        saveHighScoreToFile();
        return;
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line;
    }
    file.close();
    
    if (content.empty()) {
        m_highScore = 0.0f;
        saveHighScoreToFile();
        return;
    }
    
    m_highScore = parseJsonString(content);
}

void ScoreManager::saveHighScoreToFile() {
    std::ofstream file(SCORE_FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to save high score to file: " << SCORE_FILE_PATH << std::endl;
        return;
    }
    
    std::string jsonContent = createJsonString(m_highScore);
    file << jsonContent;
    file.close();
}

std::string ScoreManager::createJsonString(float score) {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"highScore\": " << score << ",\n";
    oss << "  \"lastUpdated\": \"" << __DATE__ << " " << __TIME__ << "\"\n";
    oss << "}";
    return oss.str();
}

float ScoreManager::parseJsonString(const std::string& jsonContent) {
    // 简单的JSON解析，查找 "highScore": 后面的数字
    size_t pos = jsonContent.find("\"highScore\":");
    if (pos == std::string::npos) {
        return 0.0f;
    }
    
    // 找到冒号后的数字
    pos = jsonContent.find(':', pos);
    if (pos == std::string::npos) {
        return 0.0f;
    }
    
    // 跳过冒号和空格
    pos++;
    while (pos < jsonContent.length() && (jsonContent[pos] == ' ' || jsonContent[pos] == '\t')) {
        pos++;
    }
    
    // 提取数字
    std::string numberStr;
    while (pos < jsonContent.length() && 
           (std::isdigit(jsonContent[pos]) || jsonContent[pos] == '.' || jsonContent[pos] == '-')) {
        numberStr += jsonContent[pos];
        pos++;
    }
    
    try {
        return std::stof(numberStr);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing high score: " << e.what() << std::endl;
        return 0.0f;
    }
}

void ScoreManager::ensureDirectoryExists(const std::string& filePath) {
    std::filesystem::path path(filePath);
    std::filesystem::path directory = path.parent_path();
    
    if (!directory.empty() && !std::filesystem::exists(directory)) {
        try {
            std::filesystem::create_directories(directory);
            std::cout << "Created directory: " << directory << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error creating directory " << directory << ": " << e.what() << std::endl;
        }
    }
}
