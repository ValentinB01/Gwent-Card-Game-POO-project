#include "GUI/GameWindow.h"
#include <iostream>

int main() {
    try {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        sf::RenderWindow window(sf::VideoMode(1280, 720), "Gwent", 
                              sf::Style::Default, settings);
        window.setVerticalSyncEnabled(true);

        if (!window.setActive(true)) {
            throw std::runtime_error("Failed to activate OpenGL context");
        }

        std::cout << "OpenGL context created successfully\n";
        std::cout << "Loading game resources...\n";

        GameWindow gameWindow("Player 1", "Player 2");
        
        std::cout << "Game initialized, entering main loop...\n";
        gameWindow.run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}