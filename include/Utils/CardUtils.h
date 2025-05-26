#pragma once

#include "enums.h"
#include <string>
#include "CardUtils.tpp"

namespace CardUtils {
    template<typename EnumType>
    std::string enumToString(EnumType);

    inline std::string zoneToString(CombatZone zone) { return enumToString(zone); }
    inline std::string factionToString(Faction faction) { return enumToString(faction); }
    inline std::string weatherTypeToSymbol(WeatherType type) { return enumToString(type); }
    inline std::string heroAbilityToString(HeroAbility ability) { return enumToString(ability); }
    inline std::string abilityEffectToString(AbilityEffect effect) { return enumToString(effect); }
    inline std::string deployEffectToString(DeployEffect effect) { return enumToString(effect); }

    std::string weatherEffectDescription(WeatherType type);
    std::string getDeployEffectDescription(DeployEffect effect, int value);
    std::string getHeroAbilityDescription(HeroAbility ability, int value);
    std::string getAbilityEffectDescription(AbilityEffect effect, int value);
}
