#include "HelpPanel.h"

HelpPanel::HelpPanel(const sf::Font& font, sf::RenderWindow& window) {
    background.setSize(sf::Vector2f(290, 230));
    background.setFillColor(sf::Color(0, 0, 0, 220));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(1.f);
    background.setPosition(20, 20);

    helpText.setFont(font);
    helpText.setCharacterSize(16);
    helpText.setString(
        "How to Play:\n"
        "1. Click cards once to select them and twice\n"
        "to play them.\n"
        "2. Read the information about them by\n"
        "hovering the card.\n"
        "3. You can use each hero ability once per\n"
        "round.\n"
        "4. Pass when you don't have what to do or\n"
        "want to end the round.\n"
        "5. The round ends when both players passed\n"
        "their turn.\n"
        "6. Win 2 rounds to win the match!"
    );
    auto windowSize = window.getSize();
    helpText.setPosition(windowSize.x - background.getSize().x - 20, 20);
}

void HelpPanel::toggle() {
    visible = !visible;
}

bool HelpPanel::isVisible() const { 
    return visible; 
}

bool HelpPanel::contains(sf::Vector2f point) const {
    return background.getGlobalBounds().contains(point);
}

void HelpPanel::draw(sf::RenderWindow& window) {
    if (!visible) return;
        
    auto windowSize = window.getSize();
    background.setPosition(windowSize.x - background.getSize().x - 20, 20);        
    window.draw(background);
    window.draw(helpText);
    
}