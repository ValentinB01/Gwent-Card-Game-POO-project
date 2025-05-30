#pragma once
#include "Deck.h"
#include <memory>
// design pattern-ul builder
class DeckBuilder {
private:
    std::unique_ptr<Deck> deck;

public:
    DeckBuilder() : deck(std::make_unique<Deck>()) {}

    DeckBuilder& loadFromFile(const std::string& filename) {
        deck->loadFromJson(filename);
        return *this;
    }

    DeckBuilder& addCard(std::unique_ptr<Card> card) {
        deck->addCard(std::move(card));
        return *this;
    }

    DeckBuilder& addCardToGraveyard(std::unique_ptr<Card> card) {
        deck->addToGraveyard(std::move(card));
        return *this;
    }

    DeckBuilder& shuffle() {
        deck->shuffle();
        return *this;
    }

    std::unique_ptr<Deck> build() {
        return std::move(deck);
    }
};
