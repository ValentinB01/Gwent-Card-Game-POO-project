#include "GUI/CardRender.h"
#include "../include/Utils/CardUtils.h"
#include <iostream>
#include <sstream>


const sf::Vector2f CardRenderer::CARD_SIZE(80.0f, 135.0f);

sf::Vector2f CardRenderer::getCardSize() const {
    return CARD_SIZE;
}

CardRenderer::CardRenderer(sf::Font& font, sf::RenderWindow& window) : tooltip(font, window){
    if (!font.loadFromFile("../assets/fonts/times.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    
    factionColors = {
        {Faction::NORTH, sf::Color(70, 130, 180)},    
        {Faction::NILFGARD, sf::Color(47, 79, 79)},     
        {Faction::SCOIATAEL, sf::Color(34, 139, 34)},   
        {Faction::MONSTERS, sf::Color(139, 0, 0)},     
        {Faction::NEUTRAL, sf::Color(169, 169, 169)}    
    };

}


bool CardRenderer::loadResources() {
    auto load = [&](TripleKey key, std::string const& path) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "ERROR: cannot load " << path << "\n";
            return false;
        }
        tex.setSmooth(true);
        textures.emplace(key, std::move(tex));
        return true;
    };

    if (!cardBackTexture.loadFromFile("assets/cards/card_back.png")) {
        std::cerr << "Failed to load card back texture\n";
        return false;
    }

    bool ok = true;

    std::vector<Faction> factions = {
        Faction::NORTH, Faction::NILFGARD,
        Faction::SCOIATAEL, Faction::MONSTERS, Faction::NEUTRAL
    };

    for (auto z : {CombatZone::CLOSE, CombatZone::RANGED, CombatZone::SIEGE}) {
        for (auto t : {CardType::UNIT, CardType::HERO}) {
            for (auto f : factions) {
                std::string fn =
                    "assets/cards/" +
                    zoneName(z) +
                    typeName(t) + "_" +
                    factionName(f) +
                    ".png";
                ok &= load({z, t, f}, fn);
            }
        }
    }

    std::vector<CombatZone> abilityZones = {
        CombatZone::CLOSE, CombatZone::RANGED, 
        CombatZone::SIEGE, CombatZone::ANY
    };
    for (auto z : abilityZones) {
        std::string fn = "assets/cards/ability_" + zoneName(z) + ".png";
        ok &= load({z, CardType::ABILITY, Faction::NEUTRAL}, fn);
    }

    static const std::vector<std::pair<WeatherType, std::string>> weathers = {
        {WeatherType::BITING_FROST, "freeze"},
        {WeatherType::IMPENETRABLE_FOG, "fog"},
        {WeatherType::TORRENTIAL_RAIN, "rain"},
        {WeatherType::CLEAR_WEATHER, "clear"}
    };
    for (auto& w : weathers) {
        std::string fn = "assets/cards/weather_" + w.second + ".png";
        ok &= load({CombatZone::ANY, CardType::WEATHER, Faction::NEUTRAL, w.first}, fn);
    }

    return ok;
}

void CardRenderer::setupCardBase(sf::RectangleShape& base, Card const& card) const {
    TripleKey key;

    key.zone = card.getZone();
    key.type = card.getType();
    key.faction = card.getFaction();

    if (card.getType() == CardType::WEATHER) {
        key.weather = card.getWeatherType();
    } else {
        key.weather = WeatherType::NONE;
    }

    auto it = textures.find(key);
    if (it != textures.end()) {
        base.setTexture(&it->second);
        base.setFillColor(sf::Color::White);
    } else {
        base.setTexture(nullptr);
        base.setFillColor(sf::Color(200, 200, 200, 180));
    }

    auto colIt = factionColors.find(card.getFaction());
    if (colIt != factionColors.end()) {
        base.setOutlineColor(colIt->second);
        base.setOutlineThickness(3.f);
    } else {
        base.setOutlineColor(sf::Color(255, 255, 255, 100));
        base.setOutlineThickness(2.f);
    }
}


void CardRenderer::renderCard(sf::RenderTarget &target, const Card &card, float x, float y,
                              bool highlight, bool isCurrentPlayer)
{
    float hoverOffset = 0.f;
    float hoverScale = 1.f;
    
    if (highlight) {
        renderRoundedRectangle(target, 
            sf::FloatRect(x + 3, y + 3, CARD_SIZE.x, CARD_SIZE.y), // Offset shadow
            CARD_CORNER_RADIUS, 
            sf::Color(0, 0, 0, 50)
        );
    }


    if (highlight) {        
        float pulse = sin(hoverClock.getElapsedTime().asSeconds() * 5.f) * 0.05f;
        hoverScale += pulse;
    }

    const_cast<Card&>(card).position = sf::Vector2f(x, y);
    const_cast<Card&>(card).size = CARD_SIZE;

    renderRoundedRectangle(target, 
                         sf::FloatRect(x + CARD_ELEVATION, y + CARD_ELEVATION, 
                                      CARD_SIZE.x, CARD_SIZE.y),
                         CARD_CORNER_RADIUS, 
                         sf::Color(0, 0, 0, 100));
    
    sf::RectangleShape cardBase(CARD_SIZE);
    setupCardBase(cardBase, card);
    cardBase.setPosition(x, y + hoverOffset);
    cardBase.setScale(hoverScale, hoverScale);
    target.draw(cardBase);
    
    sf::Text titleText;
    setupCardText(titleText, card.getName(), 18, x + 15, y + 10 + hoverOffset, 
                 sf::Color::White, true);
    titleText.setFont(font);
    
    sf::RectangleShape titleBg(sf::Vector2f(titleText.getLocalBounds().width + 20, 30));
    titleBg.setPosition(x + 5, y + 5 + hoverOffset);
    titleBg.setFillColor(factionColors.at(card.getFaction()));
    titleBg.setOutlineColor(sf::Color::Black);
    titleBg.setOutlineThickness(1.f);

    
    sf::RectangleShape artArea(sf::Vector2f(CARD_SIZE.x - 20, CARD_SIZE.y * 0.5f));
    artArea.setPosition(x + 10, y + 40 + hoverOffset);
    artArea.setFillColor(sf::Color(50, 50, 50));
    artArea.setOutlineColor(sf::Color::Black);
    artArea.setOutlineThickness(1.f);
    
    if (highlight) {
        renderCardGlow(target, x, y + hoverOffset, sf::Color(255, 255, 100, 150));
    }
}


void CardRenderer::renderCardBack(sf::RenderTarget& target, float x, float y) {
    sf::Sprite cardBack(cardBackTexture);
    cardBack.setPosition(x, y);
    cardBack.setScale(
        CARD_SIZE.x / cardBack.getLocalBounds().width,
        CARD_SIZE.y / cardBack.getLocalBounds().height
    );
    
    target.draw(cardBack);
}

void CardRenderer::setupCardsText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

std::string CardRenderer::generateTooltipText(const Card& card) const {
    std::stringstream tooltip;
    
    tooltip << "Name: " << card.getName() << "\n";
    tooltip << "Faction: " << CardUtils::factionToString(card.getFaction()) << "\n";
    tooltip << "Zone: " << CardUtils::zoneToString(card.getZone()) << "\n";

    try {
        switch(card.getType()) {
            case CardType::UNIT: {
                const UnitCard* unit = dynamic_cast<const UnitCard*>(&card);
                if (unit) {
                    tooltip << "Power: " << unit->getPower() << "\n";
                    if (unit->getDeployEffect() != DeployEffect::NONE) {
                        tooltip << "Deploy: " 
                              << CardUtils::getDeployEffectDescription(
                                  unit->getDeployEffect(), 
                                  unit->getEffectValue()
                              ) << "\n";
                    }
                }
                break;
            }
            
            case CardType::HERO: {
                const HeroCard* hero = dynamic_cast<const HeroCard*>(&card);
                if (hero) {
                    tooltip << "Power: " << hero->getPower() << "\n";
                    tooltip << "Ability: " 
                    << CardUtils::getHeroAbilityDescription(
                        hero->getAbility(),
                        hero->getAbilityValue()
                    ) << "\n";
          }
          break;
      }
            
            case CardType::ABILITY: {
                const AbilityCard* ability = dynamic_cast<const AbilityCard*>(&card);
                if (ability) {
                    tooltip << "Effect: " 
                          << CardUtils::getAbilityEffectDescription(
                              ability->getEffect(),
                              ability->getEffectValue()
                          ) << "\n";
                }
                break;
            }
            
            case CardType::WEATHER: {
                const WeatherCard* weather = dynamic_cast<const WeatherCard*>(&card);
                if (weather) {
                    tooltip << "Weather Effect: " 
                          << CardUtils::weatherEffectDescription(weather->getWeatherType()) << "\n";
                }
                break;
            }
            
            default:
                tooltip << "Special card effect\n";
        }
    } catch (const std::bad_cast& e) {
        tooltip << "\n[Error: Invalid card type conversion]";
    }

    if (!card.getDescription().empty()) {
        tooltip << "\n" << card.getDescription();
    }

    return tooltip.str();
}

void CardRenderer::drawTooltip(sf::RenderTarget& target) const {
    sf::View originalView = target.getView();
    target.setView(target.getDefaultView());
        
    tooltip.draw(target);
        
    target.setView(originalView);
}


void CardRenderer::updateHover(const sf::Vector2f& mousePos, 
                             const std::vector<const Card*>& cards) {
    hoveredCard = nullptr;
    
    for (const Card* card : cards) {
        if (card->getGlobalBounds().contains(mousePos)) {
            hoveredCard = card;
            tooltip.setText(generateTooltipText(*card));
            tooltip.setPosition(mousePos.x + 15, mousePos.y + 15);
            break;
        }
    }
    tooltip.show(hoveredCard != nullptr);
}

void CardRenderer::renderCardHoverEffect(sf::RenderTarget& target, float x, float y) const {
    sf::RectangleShape shadow(CARD_SIZE);
    shadow.setPosition(x + 5, y + 10);
    shadow.setFillColor(sf::Color(0, 0, 0, 50));
    renderRoundedRectangle(target, shadow.getGlobalBounds(), CARD_CORNER_RADIUS, shadow.getFillColor());
    
    sf::ConvexShape highlight(4);
    highlight.setPoint(0, sf::Vector2f(x, y + CARD_SIZE.y * 0.7f));
    highlight.setPoint(1, sf::Vector2f(x + CARD_SIZE.x * 0.3f, y + CARD_SIZE.y * 0.3f));
    highlight.setPoint(2, sf::Vector2f(x + CARD_SIZE.x, y + CARD_SIZE.y * 0.3f));
    highlight.setPoint(3, sf::Vector2f(x + CARD_SIZE.x, y + CARD_SIZE.y * 0.7f));
    highlight.setFillColor(sf::Color(255, 255, 255, 30));
    target.draw(highlight);
}

 void CardRenderer::renderRoundedRectangle(sf::RenderTarget& target, const sf::FloatRect& rect, 
                                        float radius, const sf::Color& fillColor, 
                                        float outlineThickness, const sf::Color& outlineColor) const {
    sf::CircleShape corner(radius);
    corner.setPointCount(32);
    corner.setFillColor(fillColor);
    corner.setOutlineThickness(outlineThickness);
    corner.setOutlineColor(outlineColor);
    
    sf::RectangleShape center(sf::Vector2f(rect.width - 2 * radius, rect.height));
    center.setPosition(rect.left + radius, rect.top);
    center.setFillColor(fillColor);
    center.setOutlineThickness(outlineThickness);
    center.setOutlineColor(outlineColor);
    target.draw(center);
    
    sf::RectangleShape left(sf::Vector2f(rect.width, rect.height - 2 * radius));
    left.setPosition(rect.left, rect.top + radius);
    left.setFillColor(fillColor);
    left.setOutlineThickness(outlineThickness);
    left.setOutlineColor(outlineColor);
    target.draw(left);
    
    corner.setPosition(rect.left, rect.top);
    target.draw(corner);
    
    corner.setPosition(rect.left + rect.width - 2 * radius, rect.top);
    target.draw(corner);
    
    corner.setPosition(rect.left, rect.top + rect.height - 2 * radius);
    target.draw(corner);
    
    corner.setPosition(rect.left + rect.width - 2 * radius, 
                      rect.top + rect.height - 2 * radius);
    target.draw(corner);
 }

void CardRenderer::setupCardText(sf::Text& text,
                                const std::string& str,
                                unsigned size,
                                float x,
                                float y,
                                const sf::Color& color,
                                bool bold) const 
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
    
    if (bold) {
        text.setStyle(sf::Text::Bold);
    }
}

void CardRenderer::setupCardTexts(sf::Text& text,
                                const std::string& str,
                                unsigned size) const 
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
}

void CardRenderer::renderCardGlow(sf::RenderTarget& target,
                                 float x,
                                 float y,
                                 const sf::Color& color) const 
{
    const float glowRadius = 25.f;
    const int points = 16;
    
    sf::CircleShape glow(glowRadius, points);
    glow.setPosition(x - glowRadius, y - glowRadius);
    glow.setFillColor(color);
    target.draw(glow);
}
