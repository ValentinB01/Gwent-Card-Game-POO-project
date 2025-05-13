#include "DropdownMenu.h"

DropdownMenu::DropdownMenu(const sf::Font& font, const sf::Vector2f& position) 
    : font(font), position(position) {
    
    mainButton.setSize(sf::Vector2f(width, 35.f));
    mainButton.setFillColor(sf::Color(70, 70, 70));
    mainButton.setOutlineColor(sf::Color::White);
    mainButton.setOutlineThickness(1.f);
    mainButton.setPosition(position);

    mainButtonText.setFont(font);
    mainButtonText.setString("Options");
    mainButtonText.setCharacterSize(16);
    mainButtonText.setPosition(position.x + 10, position.y + 5);
}

void DropdownMenu::addOption(const std::string& text, std::function<void()> action) {
    MenuOption option;
    option.rect.setSize(sf::Vector2f(width, optionHeight));
    option.rect.setFillColor(sf::Color(90, 90, 90));
    option.rect.setPosition(position.x, position.y + mainButton.getSize().y + options.size() * optionHeight);
    
    option.text.setFont(font);
    option.text.setString(text);
    option.text.setCharacterSize(14);
    option.text.setPosition(position.x + 10, position.y + mainButton.getSize().y + options.size() * optionHeight + 5);
    
    option.action = action;
    options.push_back(option);

}

sf::Vector2f DropdownMenu::getPosition() const {
    return mainButton.getPosition();
}

sf::FloatRect DropdownMenu::getGlobalBounds() const {
    return mainButton.getGlobalBounds();
}

void DropdownMenu::setPosition(const sf::Vector2f& position) {
    mainButton.setPosition(position);
    mainButtonText.setPosition(position.x + 10, position.y + 5);
    
    for(size_t i = 0; i < options.size(); ++i) {
        options[i].rect.setPosition(
            position.x,
            position.y + mainButton.getSize().y + i * optionHeight
        );
        options[i].text.setPosition(
            position.x + 10,
            position.y + mainButton.getSize().y + i * optionHeight + 5
        );
    }
}

void DropdownMenu::draw(sf::RenderWindow& window) {
    window.draw(mainButton);
    window.draw(mainButtonText);
    
    if (expanded) {
        sf::RectangleShape menuBackground({
            mainButtonSize.x,
            mainButtonSize.y + options.size() * optionHeight
        });
        menuBackground.setPosition(mainButton.getPosition());
        menuBackground.setFillColor(sf::Color(70, 70, 70));
        menuBackground.setOutlineColor(sf::Color(100, 100, 100));
        menuBackground.setOutlineThickness(2.f);
        window.draw(menuBackground);

        for (const auto& option : options) {
            window.draw(option.rect);
            window.draw(option.text);
        }
    }
}

void DropdownMenu::handleClick(const sf::Vector2f& clickPos) {
        if (mainButton.getGlobalBounds().contains(clickPos)) {
            expanded = !expanded;
        } 
        else if (expanded) {
            for (auto& option : options) {
                if (option.rect.getGlobalBounds().contains(clickPos)) {
                    option.action();
                    expanded = false;
                    break;
                }
            }
        }
    }

void DropdownMenu::update(const sf::Vector2f& mousePos) {
    mainButton.setFillColor(mainButton.getGlobalBounds().contains(mousePos) 
        ? sf::Color(100, 100, 100) 
        : sf::Color(70, 70, 70));
    if (expanded) {
            for (auto& option : options) {
                option.rect.setFillColor(
                    option.rect.getGlobalBounds().contains(mousePos)
                    ? sf::Color(100, 100, 100) 
                    : sf::Color(70, 70, 70));
            }
        }

}

void DropdownMenu::clearOptions() {
    options.clear();
}

void DropdownMenu::updateLayout() {
    mainButton.setSize(mainButtonSize);
    mainButtonText.setPosition(
        mainButton.getPosition().x + 10,
        mainButton.getPosition().y + (mainButtonSize.y - mainButtonText.getLocalBounds().height)/2
    );

    for(size_t i = 0; i < options.size(); ++i) {
        options[i].rect.setSize({mainButtonSize.x, optionHeight});
        options[i].rect.setPosition(
            mainButton.getPosition().x,
            mainButton.getPosition().y + mainButtonSize.y + i * optionHeight
        );
        options[i].text.setPosition(
            options[i].rect.getPosition().x + 10,
            options[i].rect.getPosition().y + (optionHeight - options[i].text.getLocalBounds().height)/2
        );
    }
}