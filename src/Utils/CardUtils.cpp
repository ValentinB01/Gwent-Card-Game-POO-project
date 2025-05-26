#include "../include/Utils/CardUtils.h"
#include "../include/Utils/enums.h"

std::string CardUtils::weatherEffectDescription(WeatherType type) {
    switch(type) {
        case WeatherType::BITING_FROST: return "Freezing cold reduces Close combat units to 1 power";
        case WeatherType::IMPENETRABLE_FOG: return "Thick fog reduces Ranged combat units to 1 power";
        case WeatherType::TORRENTIAL_RAIN: return "Heavy rain reduces Siege combat units to 1 power";
        case WeatherType::CLEAR_WEATHER: return "Clears all weather types from all zones";
        default: return "Unknown weather effect";
    }
}

std::string CardUtils::getDeployEffectDescription(DeployEffect effect, int value) {
    switch(effect) {
        case DeployEffect::DAMAGE_RANDOM_ENEMY: 
            return "Deal " + std::to_string(value) + " damage to a random enemy.";
        case DeployEffect::BOOST_ADJACENT: 
            return "Boost adjacent units by " + std::to_string(value) + ".";
        case DeployEffect::DRAW_CARD: 
            return "Draw " + std::to_string(value) + " card(s).";
        case DeployEffect::DESTROY_WEAKEST: 
            return "Destroy weakest enemy unit.";
        case DeployEffect::MORALE_BOOST: 
            return "Boost all lowest-power units by " + std::to_string(value) + ".";
        case DeployEffect::MEDIC: 
            return "Revive last unit from graveyard.";
        default: return "";
    }
}

std::string CardUtils::getHeroAbilityDescription(HeroAbility ability, int value) {
    switch(ability) {
        case HeroAbility::COMMANDERS_HORN: 
            return "Double the power of an entire row.";
        case HeroAbility::SCORCH: 
            return "Destroy strongest enemy unit.";
        case HeroAbility::DECOY: 
            return "Return a unit to your hand.";
        case HeroAbility::ALCHEMY: 
            return "Boost strongest unit by " + std::to_string(value) + ".";
        case HeroAbility::REVENGE: 
            return "Gain +" + std::to_string(value) + " per lost round.";
        default: return "";
    }
}

std::string CardUtils::getAbilityEffectDescription(AbilityEffect effect, int value) {
    switch(effect) {
        case AbilityEffect::DAMAGE_ROW: 
            return "Damage all units in row by " + std::to_string(value)+ ".";
        case AbilityEffect::CLEAR_SKIES: 
            return "Clear weather and boost row by " + std::to_string(value)+ ".";
        case AbilityEffect::FOGLET_SPAWN: 
            return "Summon Foglet if Fog is active.";
        case AbilityEffect::COMMANDO_TRAINING: 
            return "Boost row units by " + std::to_string(value) + ".";
        case AbilityEffect::VENOM_EXTRACT: 
            return "Poison strongest enemy (" + std::to_string(value) + " damage).";
        default: return "";
    }
}
