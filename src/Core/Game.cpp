#include "../include/Core/Game.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <ctime>
#include <stdexcept>

Game::Game(const std::string& player1Name, const std::string& player2Name) 
    : players{Player(player1Name, 0), Player(player2Name, 1)},
      currentRound(0), currentPlayerIndex(0), gameOver(false) {
        resetPassStates();
      }
void Game::loadDeck(const std::string& filename) {
    try {
        deck.loadFromJson(filename);
        players[0].setDeck(&deck);
        players[1].setDeck(&deck);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load deck: " + std::string(e.what()));
    }
}
void Game::update(float deltaTime) {
    if (gameOver) return;
    
    if (haveBothPlayersPassed()) {
        calculateRoundWinner();
        if (!gameOver) {
            nextRound();
        }
    }
}

void Game::startGame() {
    if (deck.size() < 20) {
        throw std::runtime_error("Not enough cards in deck to start game");
    }

    deck.shuffle();
    
    players[0].drawCards(10);
    players[1].drawCards(10);
    
    currentRound = 1;
    currentPlayerIndex = 0;
    gameOver = false;
    resetPassStates();
    
    std::cout << "\n=== Game Started ===\n";
    std::cout << players[0].getName() << " vs " << players[1].getName() << "\n";
    std::cout << players[currentPlayerIndex].getName() << " goes first.\n";
}

void Game::activateHeroAbility(int playerId) {
    if (playerId != currentPlayerIndex) {
        throw std::runtime_error("Not your turn!");
    }

    Player& player = (playerId == 0) ? players[0] : players[1];
    Player& opponent = (playerId == 0) ? players[0] : players[1];
    
    player.activateHeroAbility(board, opponent);
}

void Game::nextRound() {
    if (gameOver) return;

    players[0].resetHeroAbilitiesForNewRound();
    players[1].resetHeroAbilitiesForNewRound();
    board.clearBoard();
    resetPassStates();
    currentRound++;
    currentPlayerIndex = (currentRound - 1) % 2;
    
    players[0].drawCards(3);
    players[1].drawCards(3);
    
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
    
    if (playerPassed[0] && playerPassed[1]) {
        calculateRoundWinner();
        if (!gameOver) {
            nextRound();
        }
    }
}

bool Game::isPlayerTurn(int playerIndex) const {
    return playerIndex == currentPlayerIndex;
}

void Game::pass(int playerIndex) {
    if (gameOver) {
        throw std::runtime_error("Game is already over");
    }
    
    if (playerIndex != currentPlayerIndex) {
        throw std::runtime_error("Not your turn!");
    }

    playerPassed[playerIndex] = true;
    std::cout << players[playerIndex].getName() << " passes." << std::endl;
    endTurn();
}

void Game::calculateRoundWinner() {
    int player1Score = 0;
    int player2Score = 0;

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
    
    for (int i = 0; i < 2; ++i) {
        std::cout << "\n" << players[i].getName() << ":\n";
        std::cout << "  Rounds won: " << players[i].getRoundsWon() << "/2\n";
        std::cout << "  Cards in hand: " << players[i].getHandSize() << "\n";
        
        std::cout << "  Board presence:\n";
        for (auto zone : {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}) {
            int power = board.getPlayerPower(i, zone);
            if (power > 0) {
                std::cout << "    " << CardUtils::zoneToString(zone) 
                          << " combat: " << power << " power\n";
            }
        }
    }
    
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

void Game::resetPassStates() {
    playerPassed[0] = false;
    playerPassed[1] = false;
}
bool Game::haveBothPlayersPassed() const {
    return playerPassed[0] && playerPassed[1];
}

const Board& Game::getBoard() const {
    return board;
}
std::string Game::getWinnerName() const {
    if (players[0].getRoundsWon() > players[1].getRoundsWon()) {
        return players[0].getName();
    } else if (players[1].getRoundsWon() > players[0].getRoundsWon()) {
        return players[1].getName();
    }
    return "Draw - No winner";
}

Player& Game::getPlayer(int index) {
    return const_cast<Player&>(static_cast<const Game*>(this)->getPlayer(index));
}