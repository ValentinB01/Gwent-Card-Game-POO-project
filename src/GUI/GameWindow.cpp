#include "../GUI/GameWindow.h"
#include "../GUI/GameUI.h"
#include <string>
#include <iostream>
#include <stdexcept>

GameWindow::GameWindow(const std::string& p1, const std::string& p2) 
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gwent", sf::Style::Default),
      game(nullptr),
      font(),
      gameUI(nullptr) 
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
        game = std::make_unique<Game>(p1, p2);
        std::cout << "5. Game instance created\n";

        std::cout << "6. Initializing GameUI...\n";
        gameUI = std::make_unique<GameUI>(*game, *cardRenderer, font);
        std::cout << "7. GameUI initialized\n";

        std::cout << "8. Loading deck...\n";
        game->loadDeck("assets/cards.json");
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
}

void GameWindow::run() {
    if (!window.isOpen()) return;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime.asSeconds());  // Pass delta time in seconds
        render();
    }
}

void GameWindow::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (game)
        {
            gameUI->handleEvent(event, window);
        }
    }
}

void GameWindow::update(float deltaTime) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    cardRenderer->updateHover(mousePos, getAllVisibleCards());
    
    game->update(deltaTime);
}

void GameWindow::render() {
    window.clear();
    window.draw(background);
    
    renderGameBoard();
    cardRenderer->drawTooltip(window);
    gameUI->render(window);
    
    window.display();
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

    float scaleX = static_cast<float>(WINDOW_WIDTH) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(WINDOW_HEIGHT) / backgroundTexture.getSize().y;
    background.setScale(scaleX, scaleY);

    if (!cardRenderer->loadResources()) {
        throw std::runtime_error("Failed to load card renderer resources");
    }
}

void GameWindow::renderGameBoard() {
    renderPlayerInfo(game->getPlayer(0), false);
    renderPlayerInfo(game->getPlayer(1), true);
    
    renderCombatZones();
    
    renderPlayerHand(game->getPlayer(0), false);
    renderPlayerHand(game->getPlayer(1), true);
}

void GameWindow::renderPlayerHand(const Player& player, bool isBottomPlayer) {
    const auto& hand = player.getHand();
    if (hand.empty()) return;
    
    float cardWidth = cardRenderer->getCardSize().x;
    float cardHeight = cardRenderer->getCardSize().y;
    float totalWidth = hand.size() * cardWidth + (hand.size() - 1) * CARD_SPACING;
    float startX = (WINDOW_WIDTH - totalWidth) / 2;
    float y = isBottomPlayer ? WINDOW_HEIGHT - cardHeight - 20 : 20;
    
    for (size_t i = 0; i < hand.size(); ++i) {
        if (isBottomPlayer) {
            cardRenderer->renderCard(window, *hand[i], 
                                  startX + i * (cardWidth + CARD_SPACING), 
                                  y,
                                  player.getSelectedCardIndex() == static_cast<int>(i), true);
        } else {
            cardRenderer->renderCardBack(window, 
                                      startX + i * (cardWidth + CARD_SPACING), 
                                      y);
        }
    }
}

void GameWindow::renderCombatZones() {
    const float zoneHeight = cardRenderer->getCardSize().y;
    const float zoneWidth = WINDOW_WIDTH - 100;
    const float centerY = WINDOW_HEIGHT / 2;
    
    sf::RectangleShape closeZone(sf::Vector2f(zoneWidth, zoneHeight));
    closeZone.setPosition(50, centerY - zoneHeight - 20);
    closeZone.setFillColor(sf::Color(70, 70, 70, 150));
    closeZone.setOutlineColor(sf::Color::White);
    closeZone.setOutlineThickness(1.f);
    window.draw(closeZone);
    
    renderCardsInZone(game->getBoard().getPlayerZone(0, CombatZone::CLOSE), 
                    60, centerY - zoneHeight - 15);
    renderCardsInZone(game->getBoard().getPlayerZone(1, CombatZone::CLOSE), 
                    60, centerY - zoneHeight - 15);
    
    sf::RectangleShape rangedZone(sf::Vector2f(zoneWidth, zoneHeight));
    rangedZone.setPosition(50, centerY);
    rangedZone.setFillColor(sf::Color(70, 70, 70, 150));
    rangedZone.setOutlineColor(sf::Color::White);
    rangedZone.setOutlineThickness(1.f);
    window.draw(rangedZone);
    
    renderCardsInZone(game->getBoard().getPlayerZone(0, CombatZone::RANGED), 60, centerY + 5);
    renderCardsInZone(game->getBoard().getPlayerZone(1, CombatZone::RANGED), 60, centerY + 5);
    
    sf::RectangleShape siegeZone(sf::Vector2f(zoneWidth, zoneHeight));
    siegeZone.setPosition(50, centerY + zoneHeight + 20);
    siegeZone.setFillColor(sf::Color(70, 70, 70, 150));
    siegeZone.setOutlineColor(sf::Color::White);
    siegeZone.setOutlineThickness(1.f);
    window.draw(siegeZone);
    
    renderCardsInZone(game->getBoard().getPlayerZone(0, CombatZone::SIEGE), 
                    60, centerY + zoneHeight + 25);
    renderCardsInZone(game->getBoard().getPlayerZone(1, CombatZone::SIEGE), 
                    60, centerY + zoneHeight + 25);
    
    sf::Text closeLabel("Close Combat", font, 16);
    closeLabel.setPosition(60, centerY - zoneHeight - 40);
    window.draw(closeLabel);
    
    sf::Text rangedLabel("Ranged Combat", font, 16);
    rangedLabel.setPosition(60, centerY - 20);
    window.draw(rangedLabel);
    
    sf::Text siegeLabel("Siege Combat", font, 16);
    siegeLabel.setPosition(60, centerY + zoneHeight);
    window.draw(siegeLabel);
}

void GameWindow::renderCardsInZone(const std::vector<std::unique_ptr<Card>>& cards, float x, float y) {
    const float cardWidth = cardRenderer->getCardSize().x;
    const float cardHeight = cardRenderer->getCardSize().y;
    const float spacing = 5.f;
    
    for (size_t i = 0; i < cards.size(); ++i) {
        cardRenderer->renderCard(window, *cards[i], 
                              x + i * (cardWidth + spacing), 
                              y, true, true);
    }
}

void GameWindow::renderPlayerInfo(const Player& player, bool isBottomPlayer) {
    sf::Text playerText(player.getName(), font, 20);
    playerText.setStyle(sf::Text::Bold);
    
    sf::Text scoreText("Rounds: " + std::to_string(player.getRoundsWon()) + "/2", font, 16);
    
    if (isBottomPlayer) {
        playerText.setPosition(20, WINDOW_HEIGHT - 60);
        scoreText.setPosition(20, WINDOW_HEIGHT - 30);
    } else {
        playerText.setPosition(20, 20);
        scoreText.setPosition(20, 50);
    }
    
    window.draw(playerText);
    window.draw(scoreText);
}

std::vector<const Card*> GameWindow::getAllVisibleCards() const {
    std::vector<const Card*> visibleCards;
    
    const auto& currentHand = game->getCurrentPlayer().getHand();
    for (const auto& card : currentHand) {
        visibleCards.push_back(card.get());
    }

    const std::array<CombatZone, 3> zones = {
        CombatZone::CLOSE,
        CombatZone::RANGED,
        CombatZone::SIEGE
    };

    for (auto zone : zones) {
        const auto& playerCards = game->getBoard().getPlayerZone(0, zone);
        for (const auto& card : playerCards) {
            visibleCards.push_back(card.get());
        }
        
        const auto& opponentCards = game->getBoard().getPlayerZone(1, zone);
        for (const auto& card : opponentCards) {
            visibleCards.push_back(card.get());
        }
    }

    return visibleCards;
}

