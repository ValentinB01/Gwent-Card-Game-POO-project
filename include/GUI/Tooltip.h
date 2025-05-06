#pragma once

#include <SFML/Graphics.hpp>

class Tooltip {
public:
    Tooltip(const sf::Font& font, sf::RenderWindow& window);
    void setText(const std::string& text);
    void setPosition(float x, float y);
    void draw(sf::RenderTarget& target) const;
    void show(bool visible);
    
private:
    sf::Text text;
    sf::RectangleShape background;
    bool isVisible = false;
};