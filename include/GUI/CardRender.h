#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include "../Card/Card.h"
#include "../Card/UnitCard.h"
#include "../Card/HeroCard.h"
#include "../Card/AbilityCard.h"
#include "../Card/WeatherCard.h"

class CardRenderer {
public:
    CardRenderer();
    
    bool loadResources();
    sf::Vector2f getCardSize() const { return sf::Vector2f(120, 180); }
    
    void renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool highlight = false);
    void renderCardBack(sf::RenderTarget& target, float x, float y);
    
    void renderCardInfo(sf::RenderTarget& target, const Card& card, float x, float y);

private:
    sf::Font font;
    sf::Texture cardBackTexture;
    sf::Texture cardBaseTexture;
    
    std::unordered_map<Faction, sf::Color> factionColors;
    std::unordered_map<CardType, sf::Color> typeColors;
    std::unordered_map<WeatherType, sf::Color> weatherColors;
    
    void setupCardBase(sf::RectangleShape& cardBase, const Card& card) const;
    void setupCardText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const;
    void renderCardPower(sf::RenderTarget& target, const Card& card, float x, float y) const;
};

#endif