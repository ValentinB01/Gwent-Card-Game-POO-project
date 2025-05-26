#pragma once

#include <string>
#include "enums.h"

namespace CardUtils {

    template<typename EnumType>
    std::string enumToString(EnumType) {
        return "Unknown Enum";
    }

    template<>
    inline std::string enumToString<CombatZone>(CombatZone zone) {
        switch (zone) {
            case CombatZone::CLOSE: return "Close";
            case CombatZone::RANGED: return "Ranged";
            case CombatZone::SIEGE: return "Siege";
            default: return "All";
        }
    }

    template<>
    inline std::string enumToString<Faction>(Faction faction) {
        switch (faction) {
            case Faction::NORTH: return "Northern Realms";
            case Faction::SCOIATAEL: return "Scoia'tael";
            case Faction::NILFGARD: return "Nilfgaard";
            case Faction::MONSTERS: return "Monsters";
            default: return "Neutral";
        }
    }

    template<>
    inline std::string enumToString<WeatherType>(WeatherType type) {
        switch (type) {
            case WeatherType::BITING_FROST: return "❄️";
            case WeatherType::IMPENETRABLE_FOG: return "🌫️";
            case WeatherType::TORRENTIAL_RAIN: return "🌧️";
            case WeatherType::CLEAR_WEATHER: return "☀️";
            default: return "�";
        }
    }

    template<>
    inline std::string enumToString<HeroAbility>(HeroAbility ability) {
        switch (ability) {
            case HeroAbility::COMMANDERS_HORN: return "Commander's Horn";
            case HeroAbility::SCORCH: return "Scorch";
            case HeroAbility::DECOY: return "Decoy";
            case HeroAbility::ALCHEMY: return "Alchemy";
            case HeroAbility::REVENGE: return "Revenge";
            default: return "Unknown";
        }
    }

    template<>
    inline std::string enumToString<AbilityEffect>(AbilityEffect effect) {
        switch (effect) {
            case AbilityEffect::DAMAGE_ROW: return "Damage Row";
            case AbilityEffect::CLEAR_SKIES: return "Clear Skies";
            case AbilityEffect::FOGLET_SPAWN: return "Foglet Spawn";
            case AbilityEffect::COMMANDO_TRAINING: return "Commando Training";
            case AbilityEffect::VENOM_EXTRACT: return "Venom Extract";
            default: return "Unknown Effect";
        }
    }

    template<>
    inline std::string enumToString<DeployEffect>(DeployEffect effect) {
        switch (effect) {
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

} 
