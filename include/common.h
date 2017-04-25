#ifndef COMMON_H
#define COMMON_H

#include <string>

namespace RulesEngine
{
    namespace Character
    {
        enum class ModifierType : int
        {
            Alchemical, //Don't stack
            Armor, //Stack with all other AC bonuses, except other Armor bonuses
            BaseAttackBonus,
            Circumstance, //Stack with all other bonuses, including other curcumstance bonuses
            Competence, //Don't stack
            Deflection, //Stacks with all other bonuses to AC, except other deflection bonuses
            Dodge, //Stack with all other bonuses to AC, even other Dodge bonuses
            Enhancement, //Don't stack
            Inherent, //Stack
            Insight, //Don't stack
            Luck, //Don't stack
            Morale, //Don't stack
            NaturalArmor, //Stacks with all other AC bonuses, except other natural armor bonuses
            Profane, //Don't stack
            Racial, //Don't stack
            Resistance, //Don't stack
            Sacred, //Don't stack
            Shield, //Stack with all other AC bonuses, except other shield bonuses
            Size, //Don't stack
            Trait, //Don't stack
            Untyped //Stack
        };

        template<typename T> struct Modifier
        {
            ModifierType modifierType = ModifierType::Untyped;
            std::string description;
            std::string sourceName;
            int modifierValue;
            bool enabled = false;
        };
    }
}

#endif
