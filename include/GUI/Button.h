#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>

class Button {
public:
    Button(const std::string& text, const sf::Font& font, 
           const sf::Vector2f& position, const sf::Vector2f& size);
    
    void setOnClick(std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    void setEnabled(bool enabled);
    bool isHovered(const sf::Vector2f& mousePos) const;
    void triggerClick();
    void setActive(bool active);
    void update(const sf::Vector2f& mousePos);

private:
    sf::RectangleShape shape;
    sf::Text label;
    std::function<void()> onClick;
    bool isEnabled = true;
    sf::Clock pulseClock;
    bool isActive = false;
};
