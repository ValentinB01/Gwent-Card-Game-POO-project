#ifndef GWENT_CARDUTILS_H
#define GWENT_CARDUTILS_H

#include "enums.h"
#include <string>

namespace CardUtils {
    std::string zoneToString(CombatZone zone);
    std::string factionToString(Faction faction);
    std::string weatherTypeToSymbol(WeatherType type);
    std::string heroAbilityToString(HeroAbility ability);
    std::string abilityEffectToString(AbilityEffect effect);
    std::string weatherEffectDescription(WeatherType type);
    std::string deployEffectToString(DeployEffect effect);

    std::string getDeployEffectDescription(DeployEffect effect, int value);
    std::string getHeroAbilityDescription(HeroAbility ability, int value);
    std::string getAbilityEffectDescription(AbilityEffect effect, int value);
}

#endif