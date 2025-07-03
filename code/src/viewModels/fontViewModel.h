#pragma once

#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>

enum class Fonts {
    MSYHBD
};

class FontViewModel {
public:
    bool initialize();

    const std::unique_ptr<sf::Font>* getFont(const Fonts& fontType) const { return &m_fonts.at(fontType); }

private:
    bool loadFont(const Fonts& fontType, const std::string& filePath);

private:
    std::map<Fonts, std::unique_ptr<sf::Font>> m_fonts;  // 存储字体
};