#include "../include/Core/Player.h"
#include "../include/Core/Board.h"
#include "../include/Core/Deck.h"
#include "../include/Card/Card.h"
#include "../include/Card/UnitCard.h"
#include "../include/Card/WeatherCard.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <limits>

Player::Player(const std::string& name, int id, int startingLifepoints)
    : name(name), lifepoints(startingLifepoints), 
      roundsWon(0), playerId(id), deck(nullptr) {}

void Player::setDeck(Deck* d) {
    deck = d;
}

void Player::drawCard() {
    if (!deck) {
        throw std::runtime_error("Player has no deck assigned");
    }

    auto card = deck->drawCard();
    if (card) {
        hand.push_back(std::move(card));
        std::cout << name << " drew a card. Hand size: " << hand.size() << std::endl;
    } else {
        std::cout << name << " cannot draw - deck is empty!" << std::endl;
    }
}

void Player::drawCards(int count) {
    for (int i = 0; i < count; ++i) {
        drawCard();
    }
}

void Player::playCard(int index, Player& opponent, Board& board) {
    if (index < 0 || index >= hand.size()) {
        throw std::out_of_range("Invalid card index");
    }

    // Get the card before moving it
    Card* card = hand[index].get();
    std::cout << name << " plays " << card->getName() 
              << " (Power: " << card->getPower() << ")" << std::endl;

    // Move the card from hand to board
    auto playedCard = std::move(hand[index]);
    hand.erase(hand.begin() + index);
    
    // Play the card
    playedCard->play(*this, opponent, board);
}

void Player::addCardToHand(std::unique_ptr<Card> card) {
    hand.push_back(std::move(card));
}

void Player::printHand() const {
    std::cout << "\n" << name << "'s hand (" << hand.size() << " cards):\n";
    for (size_t i = 0; i < hand.size(); ++i) {
        const auto& card = hand[i];
        std::cout << i << ". " << card->getName() 
                  << " (" << card->getPower() << " power) - "
                  << "Zone: " << CardUtils::zoneToString(card->getZone()) 
                  << " | Type: ";
        
        switch(card->getType()) {
            case CardType::UNIT:
                std::cout << "Unit";
                if (dynamic_cast<const UnitCard*>(card.get())->getIsSpy()) {
                    std::cout << " (Spy)";
                }
                break;
            case CardType::HERO:
                std::cout << "Hero";
                break;
            case CardType::ABILITY:
                std::cout << "Ability";
                break;
            case CardType::WEATHER:
                std::cout << "Weather";
                break;
        }
        
        std::cout << "\n   " << card->getDescription() << "\n";
    }
}

const std::string& Player::getName() const {
    return name;
}

int Player::getLifepoints() const {
    return lifepoints;
}

size_t Player::getHandSize() const {
    return hand.size();
}

int Player::getRoundsWon() const {
    return roundsWon;
}

void Player::winRound() {
    roundsWon++;
}

int Player::getPlayerId() const {
    return playerId;
}

int Player::getRoundsLost() const {
    // Assuming rounds alternate between players
    return roundsWon;
}

void Player::loseLifepoint() {
    if (lifepoints > 0) {
        lifepoints--;
        std::cout << name << " lost a lifepoint. Remaining: " << lifepoints << std::endl;
    }
}

void Player::gainLifepoint() {
    lifepoints++;
    std::cout << name << " gained a lifepoint. Total: " << lifepoints << std::endl;
}

bool Player::hasLost() const {
    return lifepoints <= 0;
}

void Player::clearHand() {
    hand.clear();
}

void Player::playCardToBoard(std::unique_ptr<Card> card, Board& board) {
    board.addCard(playerId, std::move(card));
}

std::vector<std::unique_ptr<Card>>& Player::getGraveyard(Board& board) {
    return board.getPlayerGraveyard(playerId);
}