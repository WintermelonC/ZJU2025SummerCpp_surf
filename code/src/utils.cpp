#include "include/utils.h"

std::map<Fonts, std::string> Utils::m_fontPaths = {
    {Fonts::MSJHBD, "../../assets/fonts/MSJHBD.TTC"},
    {Fonts::almmdfdk, "../../assets/fonts/almmdfdk.TTF"}
};
std::map<Textures, std::string> Utils::m_texturePaths = {
    {Textures::water, "../../assets/images/water_texture.png"},
    {Textures::start_button, "../../assets/images/start_button.png"},
    {Textures::start_icon, "../../assets/images/start_icon.png"},
    {Textures::player_center_1, "../../assets/images/player/player_center_1.png"},
    {Textures::player_center_2, "../../assets/images/player/player_center_2.png"},
    {Textures::player_center_3, "../../assets/images/player/player_center_3.png"},
    {Textures::player_left_11, "../../assets/images/player/player_left_11.png"},
    {Textures::player_left_12, "../../assets/images/player/player_left_12.png"},
    {Textures::player_left_13, "../../assets/images/player/player_left_13.png"},
    {Textures::player_left_21, "../../assets/images/player/player_left_21.png"},
    {Textures::player_left_22, "../../assets/images/player/player_left_22.png"},
    {Textures::player_left_23, "../../assets/images/player/player_left_23.png"},
    {Textures::player_right_11, "../../assets/images/player/player_right_11.png"},
    {Textures::player_right_12, "../../assets/images/player/player_right_12.png"},
    {Textures::player_right_13, "../../assets/images/player/player_right_13.png"},
    {Textures::player_right_21, "../../assets/images/player/player_right_21.png"},
    {Textures::player_right_22, "../../assets/images/player/player_right_22.png"},
    {Textures::player_right_23, "../../assets/images/player/player_right_23.png"},
    {Textures::player_stop_1, "../../assets/images/player/player_stop_1.png"},
    {Textures::player_stop_2, "../../assets/images/player/player_stop_2.png"},
    {Textures::player_stop_3, "../../assets/images/player/player_stop_3.png"},
    {Textures::heart_1, "../../assets/images/player/heart_1.png"},
    {Textures::heart_2, "../../assets/images/player/heart_2.png"},
    {Textures::power_1, "../../assets/images/player/power_1.png"},
    {Textures::power_2, "../../assets/images/player/power_2.png"}
};

std::map<Textures, sf::Texture*> Utils::m_textures;
std::map<Fonts, sf::Font*> Utils::m_fonts;

void Utils::clear() {
    for (auto& [name, texture] : Utils::m_textures) {
        delete texture;
    }
    for (auto& [name, font] : Utils::m_fonts) {
        delete font;
    }
    Utils::m_textures.clear();
    Utils::m_fonts.clear();
}

sf::Text Utils::renderText(
        const Fonts font,
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCovert,
        const bool ifCenter
    ) {
    // 获取字体指针
    sf::Font* fontPtr = Utils::getFont(font);
    sf::Text text(*fontPtr);
    if (ifCovert) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wideStr = converter.from_bytes(content);
        text.setString(wideStr);
    } else {
        text.setString(content);
    }
    text.setCharacterSize(size);
    text.setFillColor(color);
    if (ifCenter) {
        text.setOrigin(text.getLocalBounds().size / 2.0f);
    }
    text.setPosition(position);

    return text;
}

sf::Sprite Utils::renderSprite(
        const Textures texture,
        const sf::Color color,
        const sf::Vector2f position,
        const sf::Vector2f scale,
        const bool ifSmooth,
        const bool ifCenter
    ) {
    // 获取纹理指针
    sf::Texture* texturePtr = Utils::getTexture(texture);
    texturePtr -> setSmooth(ifSmooth);
    sf::Sprite sprite(*texturePtr);
    if (ifCenter) {
        sprite.setOrigin({texturePtr -> getSize().x / 2.f, texturePtr -> getSize().y / 2.f});
    }
    sprite.setPosition(position);
    sprite.setScale(scale);
    sprite.setColor(color);

    return sprite;
}

void Utils::mouseHoverButton(
        sf::Sprite& button, 
        sf::Sprite& buttonShadow, 
        const sf::RenderWindow& window,
        const sf::Vector2f offset,
        const sf::Color color
    ) {
    // 鼠标悬停变化
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    float buttonX = button.getPosition().x;
    float buttonY = button.getPosition().y;
    float buttonWidth = button.getLocalBounds().size.x * button.getScale().x;
    float buttonHeight = button.getLocalBounds().size.y * button.getScale().y;
    if (worldPos.x >= buttonX - buttonWidth / 2 && 
        worldPos.x <= buttonX + buttonWidth / 2 &&
        worldPos.y >= buttonY - buttonHeight / 2 && 
        worldPos.y <= buttonY + buttonHeight / 2) {
        // 鼠标悬停在按钮上，改变按钮颜色
        button.setColor(sf::Color(255, 255, 255));  // 改变颜色
        buttonShadow.move({0.f, 3.f});
    }
}

void Utils::loadFont(const Fonts font) {
    std::string path = m_fontPaths[font];
    sf::Font* fontPtr = new sf::Font();
    if (!fontPtr -> openFromFile(path)) {
        throw std::runtime_error("Failed to load font from " + path);
    }
    m_fonts[font] = fontPtr;
}

void Utils::loadTexture(const Textures texture) {
    std::string path = m_texturePaths[texture];
    sf::Texture* texturePtr = new sf::Texture();
    if (!texturePtr -> loadFromFile(path)) {
        throw std::runtime_error("Failed to load texture from " + path);
    }
    m_textures[texture] = texturePtr;
}

bool Utils::ifMouseOnButton(
    const sf::Vector2f& mousePos, 
    float buttonX, 
    float buttonY, 
    float buttonWidth, 
    float buttonHeight
    ) {
    return (
        mousePos.x >= buttonX - buttonWidth / 2 && 
        mousePos.x <= buttonX + buttonWidth / 2 &&
        mousePos.y >= buttonY - buttonHeight / 2 && 
        mousePos.y <= buttonY + buttonHeight / 2
    );
}

sf::Font* Utils::getFont(const Fonts font) {
    auto it = m_fonts.find(font);
    if (it != m_fonts.end()) {
        return it -> second;
    } else {
        loadFont(font);
        it = m_fonts.find(font);
        if (it != m_fonts.end()) {
            return it -> second;
        } else {
            throw std::runtime_error("Font not found: " + std::to_string(static_cast<int>(font)));
        }
    }
}

sf::Texture* Utils::getTexture(const Textures texture) {
    auto it = m_textures.find(texture);
    if (it != m_textures.end()) {
        return it -> second;
    } else {
        loadTexture(texture);
        it = m_textures.find(texture);
        if (it != m_textures.end()) {
            return it -> second;
        } else {
            throw std::runtime_error("Texture not found: " + std::to_string(static_cast<int>(texture)));
        }
    }
}
