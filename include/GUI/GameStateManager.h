#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../Core/Game.h"
#include "../include/GUI/Button.h"
#include "../include/GUI/CardRender.h"
#include "../include/GUI/GameWindow.h"

class GameStateManager {
public:
    GameStateManager(Game& game, CardRenderer& renderer);
    
    enum class State {
        MainMenu,
        DeckBuilding,
        InGame,
        Paused,
        GameOver
    };

    void changeState(State newState);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float deltaTime, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);    
    void setHandVisible(bool visible);
    void setBoardVisible(bool visible);
    
private:
    State currentState;

    Game& game;
    CardRenderer& cardRenderer;
    
    std::vector<Button> buttons;
    std::vector<sf::Text> infoTexts;
    sf::Font font;
    
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