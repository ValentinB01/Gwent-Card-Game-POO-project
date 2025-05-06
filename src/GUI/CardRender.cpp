#include "GUI/CardRender.h"
#include <iostream>
#include <sstream>


const sf::Vector2f CardRenderer::CARD_SIZE(90.0f, 150.0f);
const float CardRenderer::CARD_CORNER_RADIUS = 5.0f;
const float CardRenderer::CARD_ELEVATION = 3.0f;

sf::Vector2f CardRenderer::getCardSize() const {
    return CARD_SIZE;
}

CardRenderer::CardRenderer(sf::Font& font, sf::RenderWindow& window) : tooltip(font, window){
    if (!font.loadFromFile("../assets/fonts/times.ttf")) {
        std::cerr << "Failed to load font\n";
    }
    
    factionColors = {
        {Faction::NORTH, sf::Color(70, 130, 180)},    // Steel blue
        {Faction::NILFGARD, sf::Color(47, 79, 79)},     // Dark slate gray
        {Faction::SCOIATAEL, sf::Color(34, 139, 34)},   // Forest green
        {Faction::MONSTERS, sf::Color(139, 0, 0)},      // Dark red
        {Faction::NEUTRAL, sf::Color(169, 169, 169)}    // Dark gray
    };
    
    typeColors = {
        {CardType::UNIT, sf::Color(255, 215, 0)},       // Gold
        {CardType::HERO, sf::Color(218, 165, 32)},      // Golden rod
        {CardType::ABILITY, sf::Color(75, 0, 130)},     // Indigo
        {CardType::WEATHER, sf::Color(135, 206, 235)}   // Sky steel
    };
    
    weatherColors = {
        {WeatherType::BITING_FROST, sf::Color(173, 216, 230)},  // Light blue
        {WeatherType::IMPENETRABLE_FOG, sf::Color(211, 211, 211)}, // Light gray
        {WeatherType::TORRENTIAL_RAIN, sf::Color(65, 105, 225)}, // Royal blue
        {WeatherType::CLEAR_WEATHER, sf::Color(255, 255, 255)},  // White
        {WeatherType::SKELIGE_STORM, sf::Color(138, 43, 226)},   // Blue violet
        {WeatherType::DRAGON_DREAM, sf::Color(220, 20, 60)}      // Crimson
    };
}

bool CardRenderer::loadResources() {
    auto loadSafe = [](sf::Texture& tex, const std::string& path) -> bool {
        if (!tex.loadFromFile(path)) {
            std::cerr << "ERROR: Failed to load texture: " << path << "\n";
            return false;
        }
        tex.setSmooth(true);
        return true;
    };

    bool success = true;

    if (!loadSafe(cardBackTexture, "assets/cards/card_back.png")) success = false;
    if (!loadSafe(cardBaseTexture, "assets/cards/card_base.png")) success = false;
    if (!loadSafe(powerCircleTexture, "assets/cards/power_circle.png")) success = false;
    if (!loadSafe(cardGlowTexture, "assets/cards/card_glow.png")) success = false;
  
    cardBaseTexture.setSmooth(true);
    cardBaseTexture.setRepeated(false);

    return success;
}

void CardRenderer::renderCard(sf::RenderTarget& target, const Card& card, float x, float y, bool highlight) {
    if (powerCircleTexture.getSize().x == 0) return;
    
    sf::Sprite powerCircle(powerCircleTexture);
    powerCircle.setPosition(x, y);
    target.draw(powerCircle);

    sf::RectangleShape cardBase(getCardSize());
    setupCardBase(cardBase, card);
    cardBase.setPosition(x, y);
    
    if (highlight) {
        cardBase.setOutlineColor(sf::Color::Yellow);
        cardBase.setOutlineThickness(3.f);
    }
    
    target.draw(cardBase);
    
    sf::Text nameText;
    setupCardText(nameText, card.getName(), 14, x + 10, y + 10);
    target.draw(nameText);
    
    if (card.getType() != CardType::WEATHER && card.getType() != CardType::ABILITY) {
        renderCardPower(target, card, x + getCardSize().x - 30, y + getCardSize().y - 30);
    }
    
    renderCardInfo(target, card, x, y);
}

void CardRenderer::renderCardBack(sf::RenderTarget& target, float x, float y) {
    if (cardBackTexture.getSize().x == 0) {
        std::cerr << "Card back texture not loaded!\n";
        return;
    }
    sf::Sprite cardBack(cardBackTexture);
    cardBack.setPosition(x, y);
    cardBack.setScale(
        getCardSize().x / cardBack.getLocalBounds().width,
        getCardSize().y / cardBack.getLocalBounds().height
    );
    target.draw(cardBack);
}

void CardRenderer::renderCardInfo(sf::RenderTarget& target, const Card& card, float x, float y) {
    sf::Text typeText;
    std::string typeStr;
    
    switch(card.getType()) {
        case CardType::UNIT:
            typeStr = "Unit";
            if (dynamic_cast<const UnitCard*>(&card)->getIsSpy()) {
                typeStr += " (Spy)";
            }
            break;
        case CardType::HERO: typeStr = "Hero"; break;
        case CardType::ABILITY: typeStr = "Ability"; break;
        case CardType::WEATHER: typeStr = "Weather"; break;
    }
    
    setupCardText(typeText, typeStr, 12, x + 10, y + 30);
    target.draw(typeText);
    
    sf::Text zoneText;
    std::string zoneStr;
    switch(card.getZone()) {
        case CombatZone::CLOSE: zoneStr = "Close"; break;
        case CombatZone::RANGED: zoneStr = "Ranged"; break;
        case CombatZone::SIEGE: zoneStr = "Siege"; break;
        case CombatZone::ANY: zoneStr = "Any"; break;
    }
    
    setupCardText(zoneText, zoneStr, 12, x + 10, y + 50);
    target.draw(zoneText);
    
    if (card.getType() == CardType::WEATHER) {
        const WeatherCard* weather = dynamic_cast<const WeatherCard*>(&card);
        sf::RectangleShape weatherIcon(sf::Vector2f(20, 20));
        weatherIcon.setPosition(x + getCardSize().x - 30, y + 10);
        weatherIcon.setFillColor(weatherColors.at(weather->getWeatherType()));
        target.draw(weatherIcon);
    }
}

void CardRenderer::setupCardBase(sf::RectangleShape& cardBase, const Card& card) const {
    cardBase.setTexture(&cardBaseTexture);
    cardBase.setFillColor(factionColors.at(card.getFaction()));
    cardBase.setOutlineColor(sf::Color::Black);
    cardBase.setOutlineThickness(1.f);
}

void CardRenderer::setupCardText(sf::Text& text, const std::string& str, unsigned size, float x, float y) const {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void CardRenderer::renderCardPower(sf::RenderTarget& target, const Card& card, float x, float y) const {
    if (powerCircleTexture.getSize().x == 0) return;
    
    sf::Sprite powerCircle(powerCircleTexture);
    powerCircle.setPosition(x, y);
    target.draw(powerCircle);
    
    sf::Text powerText;
    setupCardText(powerText, std::to_string(card.getPower()), 16, x + 5, y + 5);
    powerText.setStyle(sf::Text::Bold);
    target.draw(powerText);
}

void CardRenderer::updateHover(const sf::Vector2f& mousePos, const std::vector<const Card*>& cards) {
    tooltip.show(false);
    for (const Card* card : cards) {
        if (card->getGlobalBounds().contains(mousePos)) {
            tooltip.setText(generateTooltipText(*card));
            tooltip.setPosition(mousePos.x + 15, mousePos.y + 15);
            tooltip.show(true);
            break;
        }
    }
}

std::string CardRenderer::generateTooltipText(const Card& card) const {
    std::stringstream tooltip;
    
    // Basic card info
    tooltip << "Name: " << card.getName() << "\n";
    tooltip << "Faction: " << CardUtils::factionToString(card.getFaction()) << "\n";
    tooltip << "Zone: " << CardUtils::zoneToString(card.getZone()) << "\n";
    //tooltip << "Type: " << CardUtils::cardTypeToString(card.getType()) << "\n";

    // Type-specific information
    try {
        switch(card.getType()) {
            case CardType::UNIT: {
                const UnitCard* unit = dynamic_cast<const UnitCard*>(&card);
                if (unit) {
                    tooltip << "Power: " << unit->getPower() << "\n";
                    if (unit->getDeployEffect() != DeployEffect::NONE) {
                        tooltip << "Deploy: " << CardUtils::deployEffectToString(unit->getDeployEffect());
                        if (unit->getEffectValue() > 0) {
                            tooltip << " (" << unit->getEffectValue() << ")";
                        }
                        tooltip << "\n";
                    }
                    if (unit->getIsSpy()) {
                        tooltip << "Spy: Draw 2 cards when played\n";
                    }
                }
                break;
            }
            
            case CardType::HERO: {
                const HeroCard* hero = dynamic_cast<const HeroCard*>(&card);
                if (hero) {
                    tooltip << "Power: " << hero->getPower() << "\n";
                    tooltip << "Ability: " << CardUtils::heroAbilityToString(hero->getAbility()) << "\n";
                }
                break;
            }
            
            case CardType::ABILITY: {
                const AbilityCard* ability = dynamic_cast<const AbilityCard*>(&card);
                if (ability) {
                    tooltip << "Effect: " << CardUtils::abilityEffectToString(ability->getEffect());
                    if (ability->getEffectValue() > 0) {
                        tooltip << " (" << ability->getEffectValue() << ")";
                    }
                    tooltip << "\n";
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
    tooltip.draw(target);
}