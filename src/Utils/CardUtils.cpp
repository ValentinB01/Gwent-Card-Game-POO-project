#include "../include/Utils/CardUtils.h"
#include "../include/Utils/enums.h"

std::string CardUtils::zoneToString(CombatZone zone) {
    switch(zone) {
        case CombatZone::CLOSE: return "Close";
        case CombatZone::RANGED: return "Ranged";
        case CombatZone::SIEGE: return "Siege";
        default: return "All";
    }
}

std::string CardUtils::factionToString(Faction faction) {
    switch(faction) {
        case Faction::NORTH: return "Northern Realms";
        case Faction::SCOIATAEL: return "Scoia'tael";
        case Faction::NILFGARD: return "Nilfgaard";
        case Faction::MONSTERS: return "Monsters";
        default: return "Neutral";
    }
}

std::string CardUtils::weatherTypeToSymbol(WeatherType type) {
    switch(type) {
        case WeatherType::BITING_FROST: return "❄️";
        case WeatherType::IMPENETRABLE_FOG: return "🌫️";
        case WeatherType::TORRENTIAL_RAIN: return "🌧️";
        case WeatherType::CLEAR_WEATHER: return "☀️";
        default: return "�";
    }
}

std::string CardUtils::heroAbilityToString(HeroAbility ability) {
    switch(ability) {
        case HeroAbility::COMMANDERS_HORN: return "Commander's Horn";
        case HeroAbility::SCORCH: return "Scorch";
        case HeroAbility::DECOY: return "Decoy";
        case HeroAbility::ALCHEMY: return "Alchemy";
        case HeroAbility::REVENGE: return "Revenge";
        default: return "Unknown";
    }
}

std::string CardUtils::abilityEffectToString(AbilityEffect effect) {
    switch(effect) {
        case AbilityEffect::DAMAGE_ROW: return "Damage Row";
        case AbilityEffect::CLEAR_SKIES: return "Clear Skies";
        case AbilityEffect::FOGLET_SPAWN: return "Foglet Spawn";
        case AbilityEffect::COMMANDO_TRAINING: return "Commando Training";
        case AbilityEffect::VENOM_EXTRACT: return "Venom Extract";
        default: return "Unknown Effect";
    }
}

std::string CardUtils::weatherEffectDescription(WeatherType type) {
    switch(type) {
        case WeatherType::BITING_FROST: return "Freezing cold reduces Close combat units to 1 power";
        case WeatherType::IMPENETRABLE_FOG: return "Thick fog reduces Ranged combat units to 1 power";
        case WeatherType::TORRENTIAL_RAIN: return "Heavy rain reduces Siege combat units to 1 power";
        default: return "Unknown weather effect";
    }
}

std::string CardUtils::deployEffectToString(DeployEffect effect) {
    switch(effect) {
        case DeployEffect::DAMAGE_RANDOM_ENEMY: return "Damage Random Enemy";
        case DeployEffect::BOOST_ADJACENT: return "Boost Adjacent Units";
        case DeployEffect::DRAW_CARD: return "Draw Card";
        case DeployEffect::DESTROY_WEAKEST: return "Destroy Weakest Enemy Unit";
        case DeployEffect::MEDIC: return "Revive Unit";
        case DeployEffect::CLEAR_WEATHER: return "Clear Weather";
        case DeployEffect::SPY: return "Spy";
        case DeployEffect::MORALE_BOOST: return "Morale Boost";
        default: return "NONE";
    }
}

std::string CardUtils::getDeployEffectDescription(DeployEffect effect, int value) {
    switch(effect) {
        case DeployEffect::DAMAGE_RANDOM_ENEMY: 
            return "Deal " + std::to_string(value) + " damage to a random enemy";
        case DeployEffect::BOOST_ADJACENT: 
            return "Boost adjacent units by " + std::to_string(value);
        case DeployEffect::DRAW_CARD: 
            return "Draw " + std::to_string(value) + " card(s)";
        case DeployEffect::DESTROY_WEAKEST: 
            return "Destroy weakest enemy unit";
        case DeployEffect::MORALE_BOOST: 
            return "Boost all lowest-power units by " + std::to_string(value);
        case DeployEffect::MEDIC: 
            return "Revive last unit from graveyard";
        default: return "";
    }
}

std::string CardUtils::getHeroAbilityDescription(HeroAbility ability, int value) {
    switch(ability) {
        case HeroAbility::COMMANDERS_HORN: 
            return "Double the power of an entire row";
        case HeroAbility::SCORCH: 
            return "Destroy strongest enemy unit";
        case HeroAbility::DECOY: 
            return "Return a unit to your hand";
        case HeroAbility::ALCHEMY: 
            return "Boost strongest unit by " + std::to_string(value);
        case HeroAbility::REVENGE: 
            return "Gain +" + std::to_string(value) + " per lost round";
        default: return "";
    }
}

std::string CardUtils::getAbilityEffectDescription(AbilityEffect effect, int value) {
    switch(effect) {
        case AbilityEffect::DAMAGE_ROW: 
            return "Damage all units in row by " + std::to_string(value);
        case AbilityEffect::CLEAR_SKIES: 
            return "Clear weather and boost row by " + std::to_string(value);
        case AbilityEffect::FOGLET_SPAWN: 
            return "Summon Foglet if Fog is active";
        case AbilityEffect::COMMANDO_TRAINING: 
            return "Boost row units by " + std::to_string(value);
        case AbilityEffect::VENOM_EXTRACT: 
            return "Poison strongest enemy (" + std::to_string(value) + " damage)";
        default: return "";
    }
}