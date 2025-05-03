#include "../include/Core/Player.h"
#include "../include/Core/Board.h"
#include "../include/Core/Deck.h"
#include "../include/Card/Card.h"
#include "../include/Card/UnitCard.h"
#include "../include/Card/AbilityCard.h"
#include "../include/Card/WeatherCard.h"
#include "../include/Utils/CardUtils.h"
#include "../include/Card/HeroCard.h"
#include <iostream>
#include <unordered_map>
#include <memory>
#include <limits>

void Player::selectCard(int index) {
    if (index >= 0 && index < hand.size()) {
        selectedCardIndex = index;
    }
}

void Player::deselectCard() {
    selectedCardIndex = -1;
}

int Player::getSelectedCardIndex() const {
    return selectedCardIndex;
}

const Card* Player::getSelectedCard() const {
    if (selectedCardIndex >= 0 && selectedCardIndex < hand.size()) {
        return hand[selectedCardIndex].get();
    }
    return nullptr;
}

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

    Card* card = hand[index].get();
    std::cout << name << " plays " << card->getName() 
              << " (Power: " << card->getPower() << ")" << std::endl;

    if (card->getType() == CardType::ABILITY) {
        if (auto ability = dynamic_cast<AbilityCard*>(card)) {
            ability->play(*this, opponent, board);
            discardCard(index);
            return;
        }
    }

    auto playedCard = std::move(hand[index]);
    hand.erase(hand.begin() + index);
    playedCard->play(*this, opponent, board);
}


const std::vector<std::unique_ptr<Card>>& Player::getGraveyard() const {
    return graveyard;
}

size_t Player::getGraveyardSize() const {
    return graveyard.size();
}

void Player::discardCard(size_t handIndex) {
    if (handIndex >= hand.size()) {
        throw std::out_of_range("Invalid hand index");
    }
    
    graveyard.push_back(std::move(hand[handIndex]));
    hand.erase(hand.begin() + handIndex);
    
    std::cout << name << " discarded " << graveyard.back()->getName() 
              << " to graveyard (size: " << graveyard.size() << ")\n";
}

void Player::addCardToHand(std::unique_ptr<Card> card) {
    hand.push_back(std::move(card));
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

int Player::getRoundsLost() const {
    return roundsWon;
}

void Player::winRound() {
    roundsWon++;
}

int Player::getPlayerId() const {
    return playerId;
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

void Player::playCardTBoard(std::unique_ptr<Card> card, Player& opponent, Board& board) {
    if (auto unit = dynamic_cast<UnitCard*>(card.get())) {
        if (unit->getIsSpy()) {
            board.addCard(opponent.getPlayerId(), std::move(card));
        }
    }
    return;
}

void Player::playCardToBoard(std::unique_ptr<Card> card, Board& board) {
    board.addCard(playerId, std::move(card));
}

std::vector<std::unique_ptr<Card>>& Player::getGraveyard(Board& board) {
    return board.getPlayerGraveyard(playerId);
}

void Player::resetHeroAbilitiesForNewRound() {
    usedHeroAbilitiesThisRound.clear();
}

bool Player::canUseHeroAbility() const {
    for (const auto& card : hand) {
        if (card->getType() == CardType::HERO) {
            const HeroCard* hero = dynamic_cast<const HeroCard*>(card.get());
            if (hero && canUseHeroAbility(hero->getName())) {
                return true;
            }
        }
    }
    return false;
}
bool Player::canUseHeroAbility(const std::string& heroName) const {
    return usedHeroAbilitiesThisRound.find(heroName) == usedHeroAbilitiesThisRound.end();
}

void Player::markHeroAbilityUsed(const std::string& heroName) {
    usedHeroAbilitiesThisRound.insert(heroName);
}

void Player::activateHeroAbility(Board& board, Player& opponent) {
    std::vector<std::reference_wrapper<HeroCard>> availableHeroes;
    
    for (auto zone : {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}) {
        auto& zoneCards = board.getPlayerZone(getPlayerId(), zone);
        for (auto& card : zoneCards) {
            if (card->getType() == CardType::HERO) {
                HeroCard* hero = dynamic_cast<HeroCard*>(card.get());
                if (hero && canUseHeroAbility(hero->getName())) {
                    availableHeroes.push_back(*hero);
                }
            }
        }
    }

    if (availableHeroes.empty()) {
        std::cout << "No hero abilities available to activate this round.\n";
        return;
    }

    std::cout << "Available hero abilities:\n";
    for (size_t i = 0; i < availableHeroes.size(); ++i) {
        auto& hero = availableHeroes[i].get();
        std::cout << i+1 << ". " << hero.getName() << " - " 
                  << CardUtils::heroAbilityToString(hero.getAbility()) << "\n";
    }

    int choice = 0;
    while (choice < 1 || choice > availableHeroes.size()) {
        std::cout << "Choose a hero ability to activate (1-" << availableHeroes.size() << "): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    try {
        availableHeroes[choice-1].get().activateAbility(*this, opponent, board);
    } catch (const std::exception& e) {
        std::cerr << "Error activating ability: " << e.what() << "\n";
    }
}