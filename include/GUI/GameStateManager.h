#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../Core/Game.h"
#include "GUI/Button.h"
#include "GUI/CardRender.h"

class GameStateManager {
public:
    GameStateManager(Game& game, CardRenderer& renderer);
    
    void handleEvent(const sf::Event& event);
    void update();
    void render(sf::RenderWindow& window);
    
    void setHandVisible(bool visible);
    void setBoardVisible(bool visible);
    
private:
    Game& game;
    CardRenderer& cardRenderer;
    
    std::vector<Button> buttons;
    std::vector<sf::Text> infoTexts;
    
    bool handVisible;
    bool boardVisible;
    int selectedCardIndex;
    
    void createUIElements();
    void updateUIElements();
    void renderHand(sf::RenderWindow& window);
    void renderBoard(sf::RenderWindow& window);
    void renderPlayerInfo(sf::RenderWindow& window);
    
    void onPassClicked();
    void onCardSelected(int index);
    void onHeroAbilityClicked();
};