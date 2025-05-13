#include "../GUI/GameUI.h"
#include <iostream>

GameUI::GameUI(Game& game, CardRenderer& renderer, const sf::Font& font) 
    : game(game), cardRenderer(renderer), font(font) {
    messageText.setFont(font);
    messageText.setCharacterSize(20);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(20, 20);
    
    messageBackground.setFillColor(sf::Color(0, 0, 0, 180));
    
}

void GameUI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!window.isOpen()) return;

    for (auto& button : buttons) {
        button.handleEvent(event, window);
    }
    
    if (event.type == sf::Event::MouseButtonReleased && 
        event.mouseButton.button == sf::Mouse::Left) {
        const Player& currentPlayer = game.getCurrentPlayer();
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(
            event.mouseButton.x, event.mouseButton.y));
        
        int clickedIndex = -1;
        if (isCardClicked(mousePos, currentPlayer, clickedIndex, window)) {
            if (currentPlayer.getSelectedCardIndex() == clickedIndex) {
                try {
                    game.playCard(currentPlayer.getPlayerId(), clickedIndex);
                    const_cast<Player&>(currentPlayer).deselectCard();
                } catch (const std::exception& e) {
                    showMessage(e.what());
                }
            } else {
                const_cast<Player&>(currentPlayer).selectCard(clickedIndex);
            }
        } else {
            const_cast<Player&>(currentPlayer).deselectCard();
        }
    }
    
}

void GameUI::update(const sf::RenderWindow& window) {
    if (messageTimer > 0) {
        messageTimer -= 1.0f/60.0f; 
        if (messageTimer <= 0) {
            messageTimer = 0;
            messageText.setString("");
        }
    }
    
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (auto& button : buttons) {
        button.update(mousePos);
    }
    
}

void GameUI::render(sf::RenderWindow& window) {
    if (messageTimer > 0) {
        messageBackground.setSize(sf::Vector2f(
            messageText.getLocalBounds().width + 40,
            messageText.getLocalBounds().height + 20
        ));
        messageBackground.setPosition(
            messageText.getPosition().x - 20,
            messageText.getPosition().y - 10
        );
        window.draw(messageBackground);
        window.draw(messageText);
    }
    
    for (const auto& button : buttons) {
        button.draw(window);
    }
    
    renderSelectedCardHighlight(window, game.getCurrentPlayer());
}

void GameUI::showMessage(const std::string& message, float duration) {
    messageText.setString(message);
    messageTimer = duration;
}

void GameUI::onPassClicked() {
    try {
        game.pass(game.getCurrentPlayer().getPlayerId());
        showMessage("You passed this round");
    } catch (const std::exception& e) {
        showMessage(e.what());
    }
}

void GameUI::onEndTurnClicked() {
    game.endTurn();
    showMessage("Turn ended");
}

void GameUI::onHelpClicked() {
    showMessage("Gwent Help:\n- Click cards to select\n- Click again to play\n- Pass to end your round\n- Hero cards have special abilities", 5.0f);
}

bool GameUI::isCardClicked(const sf::Vector2f& mousePos, const Player& player, 
                          int& clickedIndex, const sf::RenderWindow& window) const {
    const auto& hand = player.getHand();
    if (hand.empty()) return false;
    
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
            clickedIndex = static_cast<int>(i);
            return true;
        }
    }
    return false;
}

void GameUI::renderSelectedCardHighlight(sf::RenderWindow& window, const Player& player) const {
    int selectedIndex = player.getSelectedCardIndex();
    if (selectedIndex < 0 || selectedIndex >= player.getHand().size()) return;
    
    float cardWidth = cardRenderer.getCardSize().x;
    float cardHeight = cardRenderer.getCardSize().y;
    float spacing = 10.f;
    float totalWidth = player.getHand().size() * cardWidth + (player.getHand().size() - 1) * spacing;
    float startX = (window.getSize().x - totalWidth) / 2;
    float y = window.getSize().y - cardHeight - 20;
    
    sf::RectangleShape highlight(sf::Vector2f(cardWidth + 10, cardHeight + 10));
    highlight.setPosition(startX + selectedIndex * (cardWidth + spacing) - 5, y - 5);
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineColor(sf::Color::Yellow);
    highlight.setOutlineThickness(3.f);
    window.draw(highlight);
}