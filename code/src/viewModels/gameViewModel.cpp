#include "gameViewModel.h"

GameViewModel::GameViewModel() {
    loadHighScoreFromFile();
}
void GameViewModel::subscribeToNotifications() {
    auto& notificationCenter = NotificationCenter::getInstance();
    // 订阅游戏重置通知
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
    // 订阅碰撞通知以更新游戏状态
    notificationCenter.subscribe(NotificationType::DamageCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::PowerCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::HealthCollision, shared_from_this());
}

void GameViewModel::update(const sf::Vector2u& windowSize) {
    if (m_gameModel.getGameState() != Config::GameState::playing) {
        return;
    }
    if (*m_playerHP <= 0 && m_playerHP != nullptr) {
        // 游戏结束时，检查并更新最高分
        if (m_gameModel.getScore() > m_highScore) {
            m_highScore = m_gameModel.getScore();
            saveHighScoreToFile();
        }
        m_gameModel.setGameState(Config::GameState::gameOver);
        return;
    }
    m_gameModel.update(*m_playerVelocity);
    updateWater(*m_playerVelocity);
}

void GameViewModel::updateWater(const sf::Vector2f& playerVelocity) {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= playerVelocity * Config::Game::PARALLAX_FACTOR;
    
    if (m_waterOffset.x <= 0) {
        m_waterOffset.x += m_waterSize;
    } else if (m_waterOffset.x >= m_waterSize) {
        m_waterOffset.x -= m_waterSize;
    }
    
    if (m_waterOffset.y <= -m_waterSize) {
        m_waterOffset.y += m_waterSize;
    }
}

void GameViewModel::resetGame() {
    //  发送游戏重置通知
    NotificationCenter::getInstance().postGameReset(true, true, true);
}

void GameViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            
            // 重置游戏模型
            if (resetData.resetScore) {
                m_gameModel.reset();
            }
            
            // 重置水面位置
            m_waterOffset = {0, 0};
            break;
        }
        default:
            break;
    }
}

void GameViewModel::loadHighScoreFromFile() {
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

void GameViewModel::ensureDirectoryExists(const std::string& filePath) {
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

void GameViewModel::saveHighScoreToFile() {
    std::ofstream file(SCORE_FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to save high score to file: " << SCORE_FILE_PATH << std::endl;
        return;
    }
    
    std::string jsonContent = createJsonString(m_highScore);
    file << jsonContent;
    file.close();
}

float GameViewModel::parseJsonString(const std::string& jsonContent) {
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

std::string GameViewModel::createJsonString(float score) {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"highScore\": " << score << ",\n";
    oss << "  \"lastUpdated\": \"" << __DATE__ << " " << __TIME__ << "\"\n";
    oss << "}";
    return oss.str();
}
