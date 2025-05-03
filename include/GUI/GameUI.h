#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
#include "Core/Game.h"
#include "GUI/CardRender.h"
#include "GUI/Button.h"

class GameUI {
public:
    GameUI(Game& game, CardRenderer& renderer, const sf::Font& font);
    GameUI(const GameUI&) = delete;
    GameUI& operator=(const GameUI&) = delete;
    
    GameUI(GameUI&&) = default;
    GameUI& operator=(GameUI&&) = default;
    ~GameUI() = default;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    
    void showMessage(const std::string& message, float duration = 3.0f);
    void updateButtonStates(const Player& currentPlayer);
    
private:
    Game& game;
    CardRenderer& cardRenderer;
    const sf::Font& font;
    
    std::vector<Button> buttons;
    sf::Text messageText;
    sf::RectangleShape messageBackground;
    float messageTimer = 0.0f;
    
    void createUIElements();
    void onPassClicked();
    void onHeroAbilityClicked();
    void onEndTurnClicked();
    void onHelpClicked();
    
    bool isCardClicked(const sf::Vector2f& mousePos, const Player& player, 
        int& clickedIndex, const sf::RenderWindow& window) const;

    void renderSelectedCardHighlight(sf::RenderWindow& window, const Player& player) const;
};