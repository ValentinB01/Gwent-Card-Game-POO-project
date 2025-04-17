#include "../include/Utils/CardUtils.h"
#include "../include/Utils/enums.h"

std::string CardUtils::zoneToString(CombatZone zone) {
    switch(zone) {
        case CombatZone::CLOSE: return "Close";
        case CombatZone::RANGED: return "Ranged";
        case CombatZone::SIEGE: return "Siege";
        default: return "Any";
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
        case WeatherType::SKELIGE_STORM: return "⚡";
        case WeatherType::DRAGON_DREAM: return "🔥";
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
        case WeatherType::SKELIGE_STORM: return "Storm damages random units";
        case WeatherType::DRAGON_DREAM: return "Dragon fire damages strongest units";
        default: return "Unknown weather effect";
    }
}