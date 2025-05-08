#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Core/Game.h"
#include "Tooltip.h"
#include "CardRender.h"
#include "GameUI.h"
#include "DropdownMenu.h"
#include "HelpPanel.h"

class Game;
class CardRenderer;
class GameWindow {
public:
    GameWindow(const std::string& player1Name, const std::string& player2Name);
    void run();
    void updateHoverState();
    bool isWindowValid() const { return window.isOpen(); }; 
    void handleEvent(const sf::Event& event);


private:
    sf::FloatRect getHandCardPosition(const Player& player, int index) const;

    void showHeroAbilities();
    
    int currentPlayerIndex;
    void processEvents();
    void update(float deltaTime);
    void render();
    void loadResources();
    std::vector<const Card*> getAllVisibleCards() const;

    void syncPlayerIndex(); 
    void renderGameBoard();
    void renderPlayerHand(const Player& player, bool isCurrentPlayer);
    void renderCombatZones();
    void renderPlayerInfo();
    void renderCardsInZone(const std::vector<std::unique_ptr<Card>>& cards, float x, float y);
    void handleCardSelection(const sf::Vector2f& mousePos);
    struct GameMessage {
        std::string text;
        float duration;
        float elapsed;
        sf::Text displayText;
    };

    
    std::vector<GameMessage> activeMessages;
    
    void addMessage(const std::string& text, float duration = 3.0f);
    void updateMessages(float deltaTime);
    void renderMessages();

    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::RenderWindow window;
    std::unique_ptr<Game> game;
    std::unique_ptr<CardRenderer> cardRenderer;
    std::unique_ptr<GameUI> gameUI;
    std::string getWinnerName() const;
    
    DropdownMenu mainMenu;
    HelpPanel helpPanel;
    

    bool loadBackground();
    sf::Color getWeatherColor(WeatherType type);

    sf::Texture zoneBackgrounds[3];
    sf::RectangleShape zoneDividers[3];
    sf::Text zonePowerTexts[3][2];
    sf::Text messageText;
    sf::RectangleShape messageBackground;
    float messageTimer = 0.f;

    void loadZoneAssets();
    void renderZoneScores();

    sf::Clock pulseClock;
    sf::Clock gameClock;
    sf::Text turnText;

    void renderTurnGlow();
    void showMessage(const std::string& message, float duration = 3.0f);
    void updateUIState();
    
    static constexpr unsigned int WINDOW_WIDTH = 1580;
    static constexpr unsigned int WINDOW_HEIGHT = 920;
    static constexpr float CARD_SPACING = 10.f;
};

#endif