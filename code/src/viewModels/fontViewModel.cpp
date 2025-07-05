#include "fontViewModel.h"

FontViewModel::FontViewModel() {
    initialize();
}

bool FontViewModel::initialize() {
    std::string assetsPath = "../../assets/";
    std::string fontPath = assetsPath + "fonts/";
    // 加载字体文件
    bool success = true;

    success &= loadFont(Fonts::MSYHBD, fontPath + "MSYHBD.TTC");

    return success;
}

bool FontViewModel::loadFont(const Fonts& fontType, const std::string& filePath) {
    auto font = std::make_unique<sf::Font>();
    if (!font->openFromFile(filePath)) {
        std::cerr << "Failed to load font from " << filePath << std::endl;
        return false;
    }
    
    m_fonts[fontType] = std::move(font);
    return true;
}