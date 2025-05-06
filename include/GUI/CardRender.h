#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include "../Card/Card.h"
#include "../Card/UnitCard.h"
#include "../Card/HeroCard.h"
#include "../GUI/Tooltip.h"
#include "../Card/AbilityCard.h"
#include "../Card/WeatherCard.h"
#include "../include/Utils/CardUtils.h"

class CardRenderer {
public:
    explicit CardRenderer(sf::Font& font, sf::RenderWindow& window);
    bool loadResources();
    void renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool highlight);
    void updateHover(const sf::Vector2f& mousePos, const std::vector<const Card*>& cards);
    void drawTooltip(sf::RenderTarget& target) const;
    std::string generateTooltipText(const Card& card) const;
    sf::Vector2f getCardSize() const;
    void renderCardBack(sf::RenderTarget& target, float x, float y);


    static const sf::Vector2f CARD_SIZE;
    static const float CARD_CORNER_RADIUS;
    static const float CARD_ELEVATION;
    
    
private:
    Tooltip tooltip;
    sf::Font font;
    sf::Texture cardBackTexture;
    sf::Texture cardBaseTexture;

    
    std::unordered_map<Faction, sf::Color> factionColors;
    std::unordered_map<CardType, sf::Color> typeColors;
    std::unordered_map<WeatherType, sf::Color> weatherColors;
    
    sf::Texture powerCircleTexture;
    sf::Texture cardGlowTexture;
    std::map<Faction, sf::Texture> factionTextures;

    void setupCardBase(sf::RectangleShape& cardBase, const Card& card) const;
    void setupCardText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const;
    void renderCardPower(sf::RenderTarget& target, const Card& card, float x, float y) const;
    void renderCardInfo(sf::RenderTarget& target, const Card& card, float x, float y);

};

#endif