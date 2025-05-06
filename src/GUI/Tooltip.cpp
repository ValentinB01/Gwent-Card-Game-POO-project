#include "../include/GUI/Tooltip.h"

Tooltip::Tooltip(const sf::Font& font, sf::RenderWindow& window) {
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    
    background.setFillColor(sf::Color(0, 0, 0, 220));
    background.setOutlineColor(sf::Color(150, 150, 150));
    background.setOutlineThickness(1.f);
}

void Tooltip::setText(const std::string& text) {
    this->text.setString(text);
    
    sf::FloatRect bounds = this->text.getLocalBounds();
    background.setSize(sf::Vector2f(
        bounds.width + 20.f,
        bounds.height + 15.f
    ));
}

void Tooltip::setPosition(float x, float y) {
    background.setPosition(x, y);
    text.setPosition(x + 10.f, y + 7.f);
}

void Tooltip::draw(sf::RenderTarget& target) const {
    if (!isVisible) return;
    target.draw(background);
    target.draw(text);
}

void Tooltip::show(bool visible) {
    if (this == nullptr) return;
    isVisible = visible;
}