# Gwent Card Game Simulation

[![C++ Version](https://img.shields.io/badge/C++-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![SFML](https://img.shields.io/badge/Library-SFML%202.5-green.svg)](https://www.sfml-dev.org/)
[![Build](https://img.shields.io/badge/Build-CMake-orange.svg)](https://cmake.org/)

A strategic, turn-based card game simulation inspired by the Witcher universe. This project focuses on high-level **C++ Software Architecture**, implementing complex game logic and an interactive Graphical User Interface (GUI).


## Game Overview

Gwent is a high-stakes duel where two players clash on a battlefield divided into three tactical rows: **Close Combat**, **Ranged**, and **Siege**. 

* **Objective**: Outscore your opponent in two out of three rounds.
* **Strategy**: Manage your limited hand wisely—there is no automatic card draw between rounds.
* **Mechanics**: Deploy unit cards, powerful heroes, weather effects, and special abilities to manipulate the battlefield's score.


## Key Features

### Interactive GUI (SFML)
* **Dynamic Board**: A 6-zone interactive layout (3 rows per player) with real-time score calculation.
* **Visual Fidelity**: Custom textures for different factions and card types (Units, Heroes, Weather, Abilities).
* **Smart Tooltips**: Real-time information display via hover-detection, showing vital card stats and effects.
* **Visual Fog of War**: Enemy cards remain face-down, while the current player's hand is fully rendered.

### Advanced OOP & Design Patterns
* **Singleton Pattern**: Ensures a single, consistent instance of the `Board` throughout the game session.
* **Builder Pattern**: Implemented via `DeckBuilder` to handle the complex construction of faction-specific decks.
* **Polymorphism & Templates**: Extensive use of abstract base classes for cards and C++ templates for optimized GUI helper functions.
* **State Management**: Orchestrates transitions between different game phases (Deploying, Passing, Round End).


## Tech Stack

| Component | Technology |
| :--- | :--- |
| **Language** | C++17 |
| **Graphics Library** | SFML (Simple and Fast Multimedia Library) |
| **Build System** | CMake |
| **Data Format** | JSON (for card attributes and deck configurations) |


🔧 Installation & Setup
Follow these instructions to build and run the game on your local machine.

1. Prerequisites
Ensure you have the following installed on your system:

C++ Compiler: A compiler supporting C++17 (e.g., g++ or clang).

CMake: Version 3.10 or higher.

SFML 2.5: The Simple and Fast Multimedia Library.

Ubuntu/Debian: sudo apt-get install libsfml-dev

MacOS: brew install sfml

Windows: Download the SDK from SFML Website and set the path in CMake.

2. Clone the Repository
Bash
git clone https://github.com/ValentinB01/Gwent-Card-Game-POO-project.git
cd Gwent-Card-Game-POO-project
3. Build the Project
We use a separate build directory to keep the project structure clean:

Bash
# Create and enter the build directory
mkdir build && cd build

# Generate build files with CMake
cmake ..

# Compile the executable
make
4. Run the Game
After a successful build, the assets folder (containing card data and textures) is automatically copied to the build directory. Run the executable:

Bash
./gwent
