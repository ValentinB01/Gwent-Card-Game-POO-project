#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class DropdownMenu {
public:
    DropdownMenu(const sf::Font& font, const sf::Vector2f& position);
    
    bool isExpanded() const { return expanded; }
    void setExpanded(bool state) { expanded = state; }

    void update(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window);
    virtual void handleClick(const sf::Vector2f& clickPos);
    void addOption(const std::string& text, std::function<void()> action);
    
    void setSize(const sf::Vector2f& size) {
        mainButtonSize = size;
        updateLayout();
    }

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    void setPosition(const sf::Vector2f& position);
    void clearOptions();
    struct MenuOption {
        sf::RectangleShape rect;
        sf::Text text;
        std::function<void()> action;
    };
    std::vector<MenuOption> options;
    bool expanded = false;


private:
    sf::Vector2f mainButtonSize;
    void updateLayout();

    

    sf::RectangleShape mainButton;
    sf::Text mainButtonText;
    const sf::Font& font;
    sf::Vector2f position;
    float optionHeight = 40.f;
    float width = 200.f;
};