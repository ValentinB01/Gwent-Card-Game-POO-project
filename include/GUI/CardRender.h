#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <cmath>
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
    void renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool highlight = false, bool isCurrentPlayer = false);
    void renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool isCurrentPlayer);
    void updateHover(const sf::Vector2f& mousePos, const std::vector<const Card*>& cards);
    void drawTooltip(sf::RenderTarget& target) const;
    std::string generateTooltipText(const Card& card) const;
    sf::Vector2f getCardSize() const;
    void renderCardBack(sf::RenderTarget& target, float x, float y);


    static const sf::Vector2f CARD_SIZE;
    static constexpr float CARD_CORNER_RADIUS = 10.f;
    static constexpr float CARD_ELEVATION = 5.f;
    
    
private:
    Tooltip tooltip;
    sf::Font font;
    sf::Texture cardBackTexture;
    sf::Texture cardBaseTexture;

    
    std::unordered_map<Faction, sf::Color> factionColors;
    std::unordered_map<CardType, sf::Color> typeColors;
    std::unordered_map<WeatherType, sf::Color> weatherColors;
    
    sf::Clock hoverClock;
    const Card* hoveredCard = nullptr;
    sf::Clock pulseClock;
    sf::Texture cardGlowTexture;
    std::map<Faction, sf::Texture> factionTextures;

    void renderCardHoverEffect(sf::RenderTarget& target, float x, float y) const;
    void renderRoundedRectangle(sf::RenderTarget& target, const sf::FloatRect& rect, 
        float radius, const sf::Color& fillColor, 
        float outlineThickness = 0.f, 
        const sf::Color& outlineColor = sf::Color::Transparent) const;
    void setupCardBase(sf::RectangleShape& cardBase, const Card& card, sf::RenderTarget& target) const;
    void setupCardBase(sf::RectangleShape& cardBase, const Card& card) const;
    void setupCardsText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const;
    void setupCardTexts(sf::Text& text, const std::string& str, unsigned size) const;
    void renderCardPower(sf::RenderTarget& target, const Card& card, float x, float y) const;
    void setupCardText(sf::Text& text, const std::string& str, unsigned size, 
                     float x, float y, const sf::Color& color = sf::Color::White, bool bold = false) const;
    void renderCardInfo(sf::RenderTarget& target, const Card& card, float x, float y) const;    
    void renderCardGlow(sf::RenderTarget& target, float x, float y, const sf::Color& color) const;
};

#endif