#include "../include/GUI/Button.h"

Button::Button(const std::string& text, const sf::Font& font, 
               const sf::Vector2f& position, const sf::Vector2f& size) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(70, 70, 70, 200));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.f);
    
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::White);
    label.setPosition(
        position.x + (size.x - label.getLocalBounds().width) / 2,
        position.y + (size.y - label.getLocalBounds().height) / 2
    );
}

void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(
            event.mouseButton.x, event.mouseButton.y));
        
        if (isHovered(mousePos)) {
            triggerClick();
        }
    }
}

void Button::update(const sf::Vector2f& mousePos) {
    if (!isEnabled) return;
    
    if (isHovered(mousePos)) {
        shape.setFillColor(sf::Color(100, 100, 100, 200));
    } else {
        shape.setFillColor(sf::Color(70, 70, 70, 200));
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}

void Button::setEnabled(bool enabled) {
    isEnabled = enabled;
    if (isEnabled) {
        label.setFillColor(sf::Color::White);
    } else {
        label.setFillColor(sf::Color(150, 150, 150));
    }
}

bool Button::isHovered(const sf::Vector2f& mousePos) const {
    return isEnabled && shape.getGlobalBounds().contains(mousePos);
}

void Button::triggerClick() {
    if (onClick && isEnabled) {
        onClick();
    }
}