#include "../include/Core/Game.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <ctime>
#include <stdexcept>

Game::Game(const std::string& player1Name, const std::string& player2Name) 
    : players{Player(player1Name, 0), Player(player2Name, 1)},
      currentRound(0), currentPlayerIndex(0), gameOver(false) {}

void Game::loadDeck(const std::string& filename) {
    try {
        deck.loadFromJson(filename);
        players[0].setDeck(&deck);
        players[1].setDeck(&deck);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load deck: " + std::string(e.what()));
    }
}

void Game::startGame() {
    if (deck.size() < 20) {
        throw std::runtime_error("Not enough cards in deck to start game");
    }

    deck.shuffle();
    
    // Initial draw
    players[0].drawCards(10);
    players[1].drawCards(10);
    
    currentRound = 1;
    currentPlayerIndex = 0;
    gameOver = false;
    
    std::cout << "\n=== Game Started ===\n";
    std::cout << players[0].getName() << " vs " << players[1].getName() << "\n";
    std::cout << players[currentPlayerIndex].getName() << " goes first.\n";
}

void Game::nextRound() {
    if (gameOver) return;

    board.clearBoard();
    currentRound++;
    currentPlayerIndex = (currentRound - 1) % 2; // Alternate who starts each round
    
    // Each player draws a card at start of round
    players[0].drawCard();
    players[1].drawCard();
    
    std::cout << "\n=== Round " << currentRound << " ===\n";
    std::cout << players[currentPlayerIndex].getName() << " starts this round.\n";
}

void Game::playCard(int playerIndex, int cardIndex) {
    if (gameOver) {
        throw std::runtime_error("Game is already over");
    }
    
    if (playerIndex != currentPlayerIndex) {
        throw std::runtime_error("Not your turn!");
    }

    Player& currentPlayer = players[playerIndex];
    Player& opponent = players[1 - playerIndex];

    if (cardIndex < 0 || cardIndex >= currentPlayer.getHandSize()) {
        throw std::out_of_range("Invalid card index");
    }

    currentPlayer.playCard(cardIndex, opponent, board);
    endTurn();
}

void Game::endTurn() {
    currentPlayerIndex = 1 - currentPlayerIndex;
    std::cout << "\nTurn passed to " << players[currentPlayerIndex].getName() << std::endl;
}

void Game::pass(int playerIndex) {
    if (gameOver) {
        throw std::runtime_error("Game is already over");
    }
    
    if (playerIndex != currentPlayerIndex) {
        throw std::runtime_error("Not your turn!");
    }

    std::cout << players[playerIndex].getName() << " passes." << std::endl;
    endTurn();
}

void Game::calculateRoundWinner() {
    int player1Score = 0;
    int player2Score = 0;

    // Calculate scores for each combat zone
    for (auto zone : {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}) {
        player1Score += board.getPlayerPower(0, zone);
        player2Score += board.getPlayerPower(1, zone);
    }

    std::cout << "\n=== Round Results ===\n";
    std::cout << players[0].getName() << ": " << player1Score << " points\n";
    std::cout << players[1].getName() << ": " << player2Score << " points\n";

    if (player1Score > player2Score) {
        players[0].winRound();
        std::cout << players[0].getName() << " wins the round!\n";
    } else if (player2Score > player1Score) {
        players[1].winRound();
        std::cout << players[1].getName() << " wins the round!\n";
    } else {
        std::cout << "Round ends in a draw! No winner.\n";
    }

    // Check for game over condition
    if (players[0].getRoundsWon() >= 2) {
        gameOver = true;
        std::cout << "\n=== Game Over ===\n";
        std::cout << players[0].getName() << " wins the game!\n";
    } else if (players[1].getRoundsWon() >= 2) {
        gameOver = true;
        std::cout << "\n=== Game Over ===\n";
        std::cout << players[1].getName() << " wins the game!\n";
    }
}

void Game::printGameState() const {
    std::cout << "\n=== Game State ===\n";
    std::cout << "Round: " << currentRound << "\n";
    std::cout << "Current turn: " << players[currentPlayerIndex].getName() << "\n";
    
    // Print player states
    for (int i = 0; i < 2; ++i) {
        std::cout << "\n" << players[i].getName() << ":\n";
        std::cout << "  Rounds won: " << players[i].getRoundsWon() << "/2\n";
        std::cout << "  Cards in hand: " << players[i].getHandSize() << "\n";
        
        // Print board presence
        std::cout << "  Board presence:\n";
        for (auto zone : {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}) {
            int power = board.getPlayerPower(i, zone);
            if (power > 0) {
                std::cout << "    " << CardUtils::zoneToString(zone) 
                          << " combat: " << power << " power\n";
            }
        }
    }
    
    // Print weather effects
    if (board.hasWeather(CombatZone::CLOSE)) {
        std::cout << "\nWeather effect in Close combat: " 
                  << CardUtils::weatherTypeToSymbol(board.getWeatherType(CombatZone::CLOSE)) << "\n";
    }
    if (board.hasWeather(CombatZone::RANGED)) {
        std::cout << "Weather effect in Ranged combat: " 
                  << CardUtils::weatherTypeToSymbol(board.getWeatherType(CombatZone::RANGED)) << "\n";
    }
    if (board.hasWeather(CombatZone::SIEGE)) {
        std::cout << "Weather effect in Siege combat: " 
                  << CardUtils::weatherTypeToSymbol(board.getWeatherType(CombatZone::SIEGE)) << "\n";
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

const Player& Game::getCurrentPlayer() const {
    return players[currentPlayerIndex];
}

const Player& Game::getPlayer(int index) const {
    if (index < 0 || index >= players.size()) {
        throw std::out_of_range("Invalid player index");
    }
    return players[index];
}

const Board& Game::getBoard() const {
    return board;
}