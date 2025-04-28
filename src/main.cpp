#include "../include/Core/Game.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>

void displayWelcome() {
    std::cout << "========================================\n";
    std::cout << "           GWENT: THE GAME              \n";
    std::cout << "========================================\n";
    std::cout << "Welcome to the console version of Gwent!\n\n";
    std::cout << "Game Rules:\n";
    std::cout << "- Win 2 out of 3 rounds to win the match\n";
    std::cout << "- Each round ends when both players pass\n";
    std::cout << "- Cards have different effects and powers\n";
    std::cout << "- Weather cards affect entire combat rows\n";
    std::cout << "========================================\n\n";
}

void displayHelp() {
    std::cout << "\nAvailable Commands:\n";
    std::cout << "1. Play card [index] - Play a card from your hand\n";
    std::cout << "2. Hero - Activate a hero ability\n";  // New option
    std::cout << "3. Pass - End your turn without playing\n";
    std::cout << "4. Hand - Show your current hand\n";
    std::cout << "5. Board - Show current board state\n";
    std::cout << "6. Help - Show this help message\n";
    std::cout << "7. Quit - Exit the game\n\n";
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

std::string getPlayerName(int playerNum) {
    std::string name;
    while (true) {
        std::cout << "Enter Player " << playerNum << " name: ";
        std::getline(std::cin, name);
        
        if (!name.empty()) {
            return name;
        }
        std::cout << "Name cannot be empty!\n";
    }
}

void handleGameInput(Game& game) {
    const Player& currentPlayer = game.getCurrentPlayer();
    
    while (true) {
        std::cout << "\n" << currentPlayer.getName() << "'s turn. Enter command (help for options): ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "1" || input.find("play") == 0) {
            // Play card (existing code)
            if (currentPlayer.getHandSize() == 0) {
                std::cout << "No cards in hand to play!\n";
                continue;
            }

            std::cout << "Enter card index (0-" << currentPlayer.getHandSize()-1 << "): ";
            int index;
            while (!(std::cin >> index) || index < 0 || index >= currentPlayer.getHandSize()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid index. Enter 0-" << currentPlayer.getHandSize()-1 << ": ";
            }
            std::cin.ignore(); // Clear newline
            
            try {
                game.playCard(currentPlayer.getPlayerId(), index);
                return;
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else if (input == "2" || input == "hero") {
            try {
                game.activateHeroAbility(currentPlayer.getPlayerId());
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
                return;
            } catch (const std::exception& e) {
                std::cout << "Error activating hero ability: " << e.what() << "\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        else if (input == "3" || input == "pass") {
            game.pass(currentPlayer.getPlayerId());
            return;
        }
        else if (input == "4" || input == "hand") {
            clearScreen();
            currentPlayer.printHand();
        }
        else if (input == "5" || input == "board") {
            clearScreen();
            game.printGameState();
        }
        else if (input == "6" || input == "help") {
            displayHelp();
        }
        else if (input == "7" || input == "quit") {
            std::cout << "Thanks for playing!\n";
            exit(0);
        }
        else {
            std::cout << "Invalid command. Type 'help' for options.\n";
        }
    }
}

void runGame() {
    clearScreen();
    displayWelcome();
    
    std::string player1Name = getPlayerName(1);
    std::string player2Name = getPlayerName(2);
    
    Game game(player1Name, player2Name);
    
    try {
        game.loadDeck("cards.json");
        game.startGame();
        displayHelp();
        
        while (!game.isGameOver()) {
            clearScreen();
            game.printGameState();
            
            // Handle turns until both players pass
            while (!game.haveBothPlayersPassed()) {
                handleGameInput(game);
                
                // Check if round ended after this move
                if (game.haveBothPlayersPassed()) {
                    break;
                }
            }
            
            // Round ended - calculate winner
            game.calculateRoundWinner();
            
            if (!game.isGameOver()) {
                std::cout << "\nPress Enter to continue to next round...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                game.nextRound();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "\nFatal Error: " << e.what() << "\n";
        exit(1);
    }
}

int main() {
    std::srand(std::time(0)); // Seed random number generator
    
    try {
        runGame();
    } catch (...) {
        std::cerr << "Unexpected error occurred!\n";
        return 1;
    }
    
    return 0;
}