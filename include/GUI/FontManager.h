#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class FontManager {
    static std::unordered_map<std::string, sf::Font> fonts;
public:
    static sf::Font& get(const std::string& path) {
        auto it = fonts.find(path);
        if (it == fonts.end()) {
            if (!fonts[path].loadFromFile(path)) {
                throw std::runtime_error("Failed to load font: " + path);
            }
        }
        return fonts[path];
    }
};