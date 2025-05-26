#include "../GUI/GameWindow.h"
#include "../GUI/GameUI.h"
#include <string>
#include <iostream>
#include <stdexcept>

GameWindow::GameWindow(const std::string& p1, const std::string& p2) 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gwent", sf::Style::Default),
      game(std::make_unique<Game>(p1, p2)),
      font(),
      gameUI(nullptr),
      currentPlayerIndex(0),
      mainMenu(font,sf::Vector2f(30, 30)),
      helpPanel(font, window)
{
    window.setVerticalSyncEnabled(true);
    try {
        std::cout << "1. Window created successfully\n";

        std::cout << "2. Loading font...\n";
        if (!font.loadFromFile("../assets/fonts/times.ttf")) {
            throw std::runtime_error("Font load failed");
        }
        std::cout << "3. Font loaded successfully\n";
        cardRenderer = std::make_unique<CardRenderer>(font, window);
        std::cout << "3.5.Rednered successfully\n";

        std::cout << "4. Creating game instance...\n";
        std::cout << "5. Game instance created\n";

        std::cout << "6. Initializing GameUI...\n";
        gameUI = std::make_unique<GameUI>(*game, *cardRenderer, font);
        std::cout << "7. GameUI initialized\n";

        std::cout << "8. Loading deck...\n";
        game->loadDeck("../assets/cards.json");
        std::cout << "9. Deck loaded\n";

        std::cout << "10. Starting game...\n";
        game->startGame();
        std::cout << "11. Game started successfully\n";

        if (!cardRenderer->loadResources()) {
            throw std::runtime_error("Card renderer initialization failed");
        }

    } catch (const std::exception& e) {
        std::cerr << "CRASH POINT: " << e.what() << std::endl;
        if (window.isOpen()) window.close();
        throw;
    }    
    mainMenu.addOption("Pass", [this]() { 
        game->pass(game->getCurrentPlayerIndex());
    });
    mainMenu.addOption("Help", [this]() { 
        helpPanel.toggle();
    });
    mainMenu.addOption("Quit", [this]() { 
        window.close();
    });
    mainMenu.setSize({120, 30});
}

void GameWindow::run() {
    if (!window.isOpen()) return;

    sf::Clock frameClock;
    while (window.isOpen()) {
        sf::Time deltaTime = frameClock.restart();
        float deltaSeconds = deltaTime.asSeconds();
        deltaSeconds = std::min(deltaSeconds, 0.1f);

        processEvents();

        if (!gameOverTriggered) {
            update(deltaSeconds);
            
            if (game->isGameOver()) {
                gameOverTriggered = true;
                gameOverClock.restart();
            }
        }

        window.clear();

        render();

        if (gameOverTriggered) {
            renderGameOverMessage();
            
            if (gameOverClock.getElapsedTime().asSeconds() >= 3.0f) {
                window.close();
            }
        }
    }
    window.display();

}

void GameWindow::renderGameOverMessage() {
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    sf::Text victoryText;
    victoryText.setFont(font);
    victoryText.setCharacterSize(40);
    victoryText.setFillColor(sf::Color::Yellow);
    victoryText.setStyle(sf::Text::Bold);
    victoryText.setString(game->getWinnerName() + " Wins!\nClosing in 3 seconds...");
    
    sf::FloatRect textBounds = victoryText.getLocalBounds();
    victoryText.setOrigin(textBounds.width/2, textBounds.height/2);
    victoryText.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    window.draw(victoryText);
}

sf::FloatRect GameWindow::getHandCardPosition(const Player& player, int index) const {
    const auto& hand = player.getHand();
    if (hand.empty() || index < 0 || index >= hand.size()) return {};
    
    const float cardWidth = cardRenderer->getCardSize().x;
    const float cardHeight = cardRenderer->getCardSize().y;
    const float spacing = 10.f;
    const float totalWidth = hand.size() * cardWidth + (hand.size() - 1) * spacing;
    const float startX = (window.getSize().x - totalWidth) / 2;
    const float y = window.getSize().y - cardHeight - 20;
    
    return {
        startX + index * (cardWidth + spacing),
        y,
        cardWidth,
        cardHeight
    };
}

void GameWindow::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (event.type == sf::Event::MouseButtonReleased && 
            event.mouseButton.button == sf::Mouse::Left) 
        {
            mainMenu.handleClick(mousePos);
        }

        if (event.type == sf::Event::Closed) {
            window.close();
        }
        mainMenu.update(mousePos);

        

        if (event.type == sf::Event::MouseButtonReleased && 
            event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                {event.mouseButton.x, event.mouseButton.y},
                window.getDefaultView()
            );

            handleCardSelection(mousePos);

            Player& currentPlayer = game->getCurrentPlayer();
            const int playerId = currentPlayer.getPlayerId();
            
            const float cardWidth = cardRenderer->getCardSize().x;
            const float cardHeight = cardRenderer->getCardSize().y;
            const float spacing = 10.f;
            const float centerY = window.getSize().y / 2.f;
            const float zoneHeight = cardHeight + 35.f;

            for (int zoneIdx = 0; zoneIdx < 3; zoneIdx++) {
                CombatZone zone = static_cast<CombatZone>(zoneIdx);
                auto& cards = game->getBoard().getPlayerZone(playerId, zone);

                float zoneY = centerY - 200.f + zoneIdx * (zoneHeight + 15.f);
                float startX = (playerId == 0) ? 60.f : window.getSize().x/2.f + 10.f;
                float startY = zoneY + 30.f;

                for (size_t i = 0; i < cards.size(); ++i) {
                    sf::FloatRect cardRect(
                        startX + i * (cardWidth + spacing),
                        startY,
                        cardWidth,
                        cardHeight
                    );

                    if (cardRect.contains(mousePos)) {
                        if (auto* hero = dynamic_cast<HeroCard*>(cards[i].get())) {
                            if (currentPlayer.canUseHeroAbility(hero->getName())) {
                                try {
                                    hero->activateAbility(currentPlayer, 
                                                        game->getOpponent(), 
                                                        game->getBoard());
                                    currentPlayer.markHeroAbilityUsed(hero->getName());
                                } catch (const std::exception& e) {
                                }
                            } else {
                            }
                        }
                    }
                }
            }
        }

        cardRenderer->updateHover(mousePos, getAllVisibleCards());
    }
}

void GameWindow::handleCardSelection(const sf::Vector2f& mousePos) {
    Player& currentPlayer = game->getCurrentPlayer();
    const auto& hand = currentPlayer.getHand();
    
    const float cardWidth = cardRenderer->getCardSize().x;
    const float cardHeight = cardRenderer->getCardSize().y;
    const float spacing = 10.f;
    const float totalWidth = hand.size() * cardWidth + (hand.size() - 1) * spacing;
    const float startX = (window.getSize().x - totalWidth) / 2;
    const float yPos = window.getSize().y - cardHeight - 20;

    for (size_t i = 0; i < hand.size(); ++i) {
        sf::FloatRect cardRect(
            startX + i * (cardWidth + spacing),
            yPos,
            cardWidth,
            cardHeight
        );

        if (cardRect.contains(mousePos)) {
            const int selectedIndex = currentPlayer.getSelectedCardIndex();
            
            if (selectedIndex == static_cast<int>(i)) {
                try {
                    game->playCard(currentPlayer.getPlayerId(), i);
                    currentPlayer.deselectCard();
                } catch (const std::exception& e) {
                }
            } else {
                currentPlayer.selectCard(i);
            }
            return;
        }
    }
    
    currentPlayer.deselectCard();
}

void GameWindow::update(float deltaTime) {
    sf::Vector2f mousePos = window.mapPixelToCoords(
        sf::Mouse::getPosition(window),
        window.getDefaultView()
    );
    cardRenderer->updateHover(mousePos, getAllVisibleCards());
    
    game->update(deltaTime);
}

void GameWindow::render() {
    window.clear(sf::Color(30, 22, 16));
    window.draw(background);
    
    const int currentIdx = game->getCurrentPlayerIndex();

    renderGameBoard();
    
    renderPlayerHand(game->getPlayer(currentIdx), true); 

    renderPlayerInfo();

    cardRenderer->drawTooltip(window);
    gameUI->render(window);
    mainMenu.draw(window);
    helpPanel.draw(window);
    window.display();
}

std::string GameWindow::getWinnerName() const {
    if (!game) {
        return "Game not initialized";
    }
    
    const Player& p1 = game->getPlayer(0);
    const Player& p2 = game->getPlayer(1);
    
    if (p1.getRoundsWon() > p2.getRoundsWon()) {
        return p1.getName();
    } else if (p2.getRoundsWon() > p1.getRoundsWon()) {
        return p2.getName();
    }
    return "Draw";
}

void GameWindow::updateUIState() 
{
        const Player& currentPlayer = game->getCurrentPlayer();
}


void GameWindow::loadResources() {
    if (!font.loadFromFile("../assets/fonts/times.ttf")) {
        std::cerr << "ERROR: Failed to load font from: assets/fonts/myfont.ttf\n";
        return; 
    }
    
    if (!backgroundTexture.loadFromFile("../assets/cards/background.jpg")) {
        throw std::runtime_error("Failed to load background");
    }
    background.setTexture(backgroundTexture);

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u texSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / texSize.x;
    float scaleY = static_cast<float>(windowSize.y) / texSize.y;
    background.setScale(scaleX, scaleY);

    if (!cardRenderer->loadResources()) {
        throw std::runtime_error("Failed to load card renderer resources");
    }
}

void GameWindow::renderTurnGlow() {
    float glow = sin(pulseClock.getElapsedTime().asSeconds() * 5) * 0.5f + 0.5f;
    sf::RectangleShape glowBar(sf::Vector2f(WINDOW_WIDTH, 3));
    glowBar.setPosition(0, game->getCurrentPlayer().getPlayerId() == 0 ? 95 : WINDOW_HEIGHT - 98);
    glowBar.setFillColor(sf::Color(255, 255, 0, static_cast<sf::Uint8>(glow * 200)));
    window.draw(glowBar);
}

void GameWindow::loadZoneAssets() {
    zoneBackgrounds[0].loadFromFile("../assets/zones/close_combat.png");
    zoneBackgrounds[1].loadFromFile("../assets/zones/ranged_combat.png");
    zoneBackgrounds[2].loadFromFile("../assets/zones/siege_combat.png");
    
}

void GameWindow::renderZoneScores() {
    const float centerY = WINDOW_HEIGHT / 2;
    const float zoneHeight = cardRenderer->getCardSize().y;
    
    for (int i = 0; i < 3; i++) {
        CombatZone zone = static_cast<CombatZone>(i);
        
        int p1Power = game->getBoard().getPlayerPower(0, zone);
        zonePowerTexts[i][0].setString(std::to_string(p1Power));
        zonePowerTexts[i][0].setPosition(70, centerY - zoneHeight - 15 + i*(zoneHeight+40));
        
        int p2Power = game->getBoard().getPlayerPower(1, zone);
        zonePowerTexts[i][1].setString(std::to_string(p2Power));
        zonePowerTexts[i][1].setPosition(WINDOW_WIDTH - 70, centerY - zoneHeight - 15 + i*(zoneHeight+40));
        
        window.draw(zonePowerTexts[i][0]);
        window.draw(zonePowerTexts[i][1]);
    }
}


void GameWindow::renderGameBoard() {
    renderPlayerInfo();
    
    renderCombatZones();
    
    renderPlayerHand(game->getPlayer(0), false);
    renderPlayerHand(game->getPlayer(1), true);
}


void GameWindow::updateHoverState() {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    std::vector<const Card*> visibleCards = getAllVisibleCards();
    cardRenderer->updateHover(mousePos, visibleCards);
}

std::vector<const Card*> GameWindow::getAllVisibleCards() const {
    std::vector<const Card*> cards;
    
    const auto& hand = game->getCurrentPlayer().getHand();
    for (const auto& card : hand) {
        cards.push_back(card.get());
    }
    
    for (int zone = 0; zone < 3; zone++) {
        const auto& zoneCards = game->getBoard().getPlayerZone(0, static_cast<CombatZone>(zone));
        for (const auto& card : zoneCards) {
            cards.push_back(card.get());
        }
        
        const auto& oppZoneCards = game->getBoard().getPlayerZone(1, static_cast<CombatZone>(zone));
        for (const auto& card : oppZoneCards) {
            cards.push_back(card.get());
        }
    }
    
    return cards;
}

void GameWindow::renderCombatZones() {
    const float zoneHeight = cardRenderer->getCardSize().y + 35.f;
    const float zoneWidth = window.getSize().x - 100;
    const float centerY = window.getSize().y / 2.f;
    
    for (int i = 0; i < 3; i++) {
        CombatZone zone = static_cast<CombatZone>(i);
        float zoneY = centerY - 200 + i * (zoneHeight + 15);
        
        sf::RectangleShape zoneRect(sf::Vector2f(zoneWidth, zoneHeight));
        zoneRect.setPosition(50, zoneY);
        zoneRect.setFillColor(sf::Color(94, 70, 44));
        zoneRect.setOutlineColor(sf::Color(150, 150, 150, 200));
        zoneRect.setOutlineThickness(1.f);
        window.draw(zoneRect);
        
        sf::Text zoneLabel(CardUtils::zoneToString(zone), font, 18);
        zoneLabel.setPosition(60, zoneY + 5);
        window.draw(zoneLabel);
        
        int p1Score = game->getBoard().getPlayerPower(0, zone);
        int p2Score = game->getBoard().getPlayerPower(1, zone);
        
        sf::Text scoreText(std::to_string(p1Score) + " - " + std::to_string(p2Score), font, 20);
        scoreText.setPosition(zoneWidth/2 + 25, zoneY + 5);
        window.draw(scoreText);
        
        renderCardsInZone(game->getBoard().getPlayerZone(0, zone), 60, zoneY + 30);
        renderCardsInZone(game->getBoard().getPlayerZone(1, zone), 
                        window.getSize().x/2 + 10, zoneY + 30);
    }
}



void GameWindow::renderCardsInZone(const std::vector<std::unique_ptr<Card>>& cards, 
                                 float startX, float startY) {
    const float cardWidth = cardRenderer->getCardSize().x;
    const float spacing = 10.f;
    
    for (size_t i = 0; i < cards.size(); ++i) {
        cardRenderer->renderCard(
            window,
            *cards[i],
            startX + i * (cardWidth + spacing),
            startY,
            false,
            false
        );
    }
}

void GameWindow::renderPlayerHand(const Player& player, bool isCurrentPlayer) {
    const auto& hand = player.getHand();
    if (hand.empty()) return;

    const float spacing = 10.f;
    const float cardWidth = cardRenderer->getCardSize().x;
    const float cardHeight = cardRenderer->getCardSize().y;

    float totalWidth = (hand.size() * cardWidth) + ((hand.size() - 1) * spacing);
    float startX = (window.getSize().x - totalWidth) / 2;
    float baseY = isCurrentPlayer ? window.getSize().y - 150.f : 50.f;
    if (isCurrentPlayer){
    for (size_t i = 0; i < hand.size(); ++i) {
        float xPos = startX + i * (cardWidth + spacing);
        float yPos = baseY;

        sf::FloatRect cardPos = getHandCardPosition(player, i);

        const_cast<Card*>(hand[i].get())->position = sf::Vector2f(xPos, yPos);
            
        cardRenderer->renderCard(
            window,
            *hand[i],
            cardPos.left,
            cardPos.top,
            (player.getSelectedCardIndex() == i),
            isCurrentPlayer
        );
    }
    }  else {
        const float CARD_WIDTH = 80.f;
        const float SPACING = 15.f;
        auto& hand = game->getOpponent().getHand();
        
        float totalWidth = (hand.size() * CARD_WIDTH) + ((hand.size() - 1) * SPACING);
        float startX = (window.getSize().x - totalWidth) / 2;
        float baseY = 70.f;  
    
        for (size_t i = 0; i < hand.size(); ++i) {
            float x = startX + i * (CARD_WIDTH + SPACING);
            float y = baseY + (i % 2) * 20.f;
            
            hand[i]->setPosition(x, y);
            cardRenderer->renderCardBack(window,x, y);
        }
        
    }
}

void GameWindow::renderPlayerInfo() {
    const float panelWidth = WINDOW_WIDTH * 0.6f;
    const float panelHeight = 60.f;
    const sf::Vector2f panelPos(
        (WINDOW_WIDTH - panelWidth) / 2, 
        10.f
    );

    sf::RectangleShape panel(sf::Vector2f(panelWidth, panelHeight));
    panel.setPosition(panelPos);
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    panel.setOutlineColor(sf::Color(100, 100, 100));
    panel.setOutlineThickness(1.f);
    window.draw(panel);

    const float columnWidth = panelWidth / 2;
    const float textStartY = panelPos.y + 10.f;
    const int currentIdx = game->getCurrentPlayerIndex();

    for (int i = 0; i < 2; ++i) {
        const Player& p = game->getPlayer(i);
        const float xOffset = panelPos.x + (i * columnWidth);
        const bool isCurrent = (i == currentIdx);

        sf::Text name(p.getName(), font, 22);
        name.setPosition(xOffset + 15.f, textStartY);
        name.setFillColor(isCurrent ? sf::Color(255, 215, 0) : sf::Color::White);
        window.draw(name);

        sf::Text rounds("Rounds: " + std::to_string(p.getRoundsWon()) + "/2", font, 16);
        rounds.setPosition(xOffset + 15.f, textStartY + 30.f);
        window.draw(rounds);

        sf::Text cards("Cards: " + std::to_string(p.getHandSize()), font, 16);
        cards.setPosition(xOffset + columnWidth - cards.getLocalBounds().width - 15.f, 
                        textStartY + 30.f);
        window.draw(cards);

        if (isCurrent) {
            sf::ConvexShape arrow(3);
            arrow.setPoint(0, sf::Vector2f(xOffset + 5.f, textStartY + 15.f));
            arrow.setPoint(1, sf::Vector2f(xOffset + 20.f, textStartY + 15.f));
            arrow.setPoint(2, sf::Vector2f(xOffset + 12.5f, textStartY + 25.f));
            arrow.setFillColor(sf::Color(255, 215, 0));
            window.draw(arrow);
        }
    }
}


void GameWindow::syncPlayerIndex() {
    if (game) { 
        currentPlayerIndex = game->getCurrentPlayerIndex();
    }
}
bool GameWindow::isWindowValid() const { 
    return window.isOpen(); 
} 
