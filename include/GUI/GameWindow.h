#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Core/Game.h"
#include "../GUI/CardRender.h"
#include "../GUI/GameUI.h"

class GameWindow {
public:
    GameWindow(const std::string& player1Name, const std::string& player2Name);
    void run();

private:
    void processEvents();
    void update();
    void render();
    void loadResources();
    
    void renderGameBoard();
    void renderPlayerHand(const Player& player, bool isBottomPlayer);
    void renderCombatZones();
    void renderPlayerInfo(const Player& player, bool isBottomPlayer);
    void renderCardsInZone(const std::vector<std::unique_ptr<Card>>& cards, float x, float y);

    sf::RenderWindow window;
    std::unique_ptr<Game> game;
    CardRenderer cardRenderer;
    std::unique_ptr<GameUI> gameUI;
    
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    
    static constexpr unsigned int WINDOW_WIDTH = 1280;
    static constexpr unsigned int WINDOW_HEIGHT = 720;
    static constexpr float CARD_SPACING = 10.f;
};

#endif