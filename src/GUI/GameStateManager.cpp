#include "../GUI/GameStateManager.h"
#include <map>
#include <iostream>

GameStateManager::GameStateManager(Game& game, CardRenderer& renderer) 
    : game(game), 
      cardRenderer(renderer),
      currentState(State::MainMenu),
      handVisible(false),
      boardVisible(false),
      selectedCardIndex(-1)
{
    if (!font.loadFromFile("../assets/fonts/times.ttf")) { 
        throw std::runtime_error("Failed to load font");
    }
    createUIElements();
}

void GameStateManager::changeState(State newState) {
    currentState = newState;
    
    switch(currentState) {
        case State::InGame:
            handVisible = true;
            boardVisible = true;
            break;
        case State::DeckBuilding:
            handVisible = true;
            boardVisible = false;
            break;
        case State::MainMenu:
        case State::Paused:
        case State::GameOver:
            handVisible = false;
            boardVisible = false;
            break;
    }
    
    updateUIElements();
}

void GameStateManager::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button.handleEvent(event, window);
    }
    
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        
        if (handVisible) {
            const auto& hand = game.getCurrentPlayer().getHand();
            float cardWidth = cardRenderer.getCardSize().x;
            float cardHeight = cardRenderer.getCardSize().y;
            float spacing = 10.f;
            float totalWidth = hand.size() * cardWidth + (hand.size() - 1) * spacing;
            float startX = (window.getSize().x - totalWidth) / 2;
            float y = window.getSize().y - cardHeight - 20;
            
            for (size_t i = 0; i < hand.size(); ++i) {
                sf::FloatRect cardRect(
                    startX + i * (cardWidth + spacing),
                    y,
                    cardWidth,
                    cardHeight
                );
                
                if (cardRect.contains(mousePos)) {
                    onCardSelected(i);
                    break;
                }
            }
        }
    }
}

void GameStateManager::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto& button : buttons) {
        button.update(mousePos);
    }
    
    // State-specific updates
    switch(currentState) {
        case State::InGame:
            // Update game logic
            break;
        case State::Paused:
            // Paused state updates
            break;
        // Other states...
    }
}

void GameStateManager::render(sf::RenderWindow& window) {
    // Render game elements based on visibility
    if (boardVisible) {
        renderBoard(window);
        renderPlayerInfo(window);
    }
    
    if (handVisible) {
        renderHand(window);
    }
    
    // Render UI elements
    for (const auto& button : buttons) {
        button.draw(window);
    }
    
    for (const auto& text : infoTexts) {
        window.draw(text);
    }
}

void GameStateManager::setHandVisible(bool visible) {
    handVisible = visible;
    updateUIElements();
}

void GameStateManager::setBoardVisible(bool visible) {
    boardVisible = visible;
    updateUIElements();
}

// Private implementation methods
void GameStateManager::createUIElements() {
    // Create common buttons
    buttons.emplace_back("Pass", font, sf::Vector2f(20, 20), sf::Vector2f(100, 40));
    buttons.back().setOnClick([this]() { onPassClicked(); });
    
    buttons.emplace_back("Hero Ability", font, sf::Vector2f(130, 20), sf::Vector2f(150, 40));
    buttons.back().setOnClick([this]() { onHeroAbilityClicked(); });
    
    // Initialize info texts
    infoTexts.emplace_back("", font, 20);
    infoTexts.back().setPosition(300, 20);
}

void GameStateManager::updateUIElements() {
    // Update button visibility based on state
    buttons[0].setEnabled(currentState == State::InGame); // Pass button
    buttons[1].setEnabled(currentState == State::InGame && 
                         game.getCurrentPlayer().canUseHeroAbility()); // Hero button
    
    // Update info text
    if (currentState == State::InGame) {
        infoTexts[0].setString(game.getCurrentPlayer().getName() + "'s Turn");
    }
}

void GameStateManager::renderHand(sf::RenderWindow& window) {
    const auto& hand = game.getCurrentPlayer().getHand();
    float cardWidth = cardRenderer.getCardSize().x;
    float cardHeight = cardRenderer.getCardSize().y;
    float spacing = 10.f;
    float totalWidth = hand.size() * cardWidth + (hand.size() - 1) * spacing;
    float startX = (window.getSize().x - totalWidth) / 2;
    float y = window.getSize().y - cardHeight - 20;
    
    for (size_t i = 0; i < hand.size(); ++i) {
        bool highlight = (selectedCardIndex == static_cast<int>(i));
        cardRenderer.renderCard(window, *hand[i], startX + i * (cardWidth + spacing), y, highlight,true); // true e in plus
    }
}

void GameStateManager::renderBoard(sf::RenderWindow& window) {
    // Render combat zones
    const float zoneHeight = cardRenderer.getCardSize().y;
    const float zoneWidth = window.getSize().x - 100;
    const float centerY = window.getSize().y / 2;
    
    // Close combat zone
    sf::RectangleShape closeZone(sf::Vector2f(zoneWidth, zoneHeight));
    closeZone.setPosition(50, centerY - zoneHeight - 20);
    closeZone.setFillColor(sf::Color(70, 70, 70, 150));
    window.draw(closeZone);
    
    // Render cards in zones...
}



void GameStateManager::onPassClicked() {
    try {
        game.pass(game.getCurrentPlayer().getPlayerId());
        infoTexts[0].setString("Player passed");
    } catch (const std::exception& e) {
        infoTexts[0].setString("Error: " + std::string(e.what()));
    }
}

void GameStateManager::onCardSelected(int index) {
    selectedCardIndex = index;
    try {
        game.playCard(game.getCurrentPlayer().getPlayerId(), index);
        selectedCardIndex = -1; // Reset after successful play
    } catch (const std::exception& e) {
        infoTexts[0].setString("Can't play that card: " + std::string(e.what()));
    }
}

void GameStateManager::onHeroAbilityClicked() {
    try {
        game.activateHeroAbility(game.getCurrentPlayer().getPlayerId());
    } catch (const std::exception& e) {
        infoTexts[0].setString("Hero ability failed: " + std::string(e.what()));
    }
}

void GameStateManager::renderPlayerInfo(sf::RenderWindow& window) {
    const Player& p1 = game.getPlayer(0);
    const Player& p2 = game.getPlayer(1);
    
    sf::Text player1Text(p1.getName(), font, 20);
    player1Text.setPosition(20, 20);
    window.draw(player1Text);
    
    sf::Text player2Text(p2.getName(), font, 20);
    player2Text.setPosition(window.getSize().x - 120, 20);
    window.draw(player2Text);
}
