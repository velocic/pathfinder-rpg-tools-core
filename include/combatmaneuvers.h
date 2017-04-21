#ifndef COMBATMANEUVERBONUS_H
#define COMBATMANEUVERBONUS_H

#include <string>

#include <abilityscores.h>
#include <baseattackbonus.h>
#include <characterdescription.h>
#include <observer.h>
#include <observersubject.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        enum class CombatManeuverModifierType : int
        {
            Circumstance, //Stack with eachother, except same source
            Competence, //Don't stack with eachother, only highest applies
            Enhancement, //Magic modifiers from weapons, don't stack
            Insight, //Does not stack, only highest applies
            Luck, //Don't stack, only highest applies
            Morale, //Don't stack, only highest applies
            Size, //Should only have one, since you can only ever be one size at once
            Racial, //Don't stack, only highest applies
            Trait, //Don't stack, only highest applies
            Untyped //Stack with eachother. Most maneuver bonuses seem to be untyped
        };

        struct CombatManeuverModifier
        {
            CombatManeuverModifierType modifierType = CombatManeuverModifierType::Untyped;
            std::string description;
            std::string sourceName;
            int modifierValue = 0;
            bool enabled = false;
        };

        class PATHFINDER_RULES_ENGINE_EXPORT CombatManeuvers :
            public ObserverSubject,
            public Observer
        {
            private:
                CharacterDescription &characterDescription;
                AbilityScores &abilityScores;
                BaseAttackBonus &baseAttackBonus;
                int totalCombatManeuverBonus = 0;
                int totalCombatManeuverDefense = 0;

                std::unordered_map<std::string, CombatManeuverModifier> CMBModifiers;
                std::unordered_map<std::string, CombatManeuverModifier> CMDModifiers;
                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;

                std::unordered_map<std::string, CombatManeuverModifier> getContributingModifiers(const std::unordered_map<std::string, CombatManeuverModifier>& rawModifierList) const;
                int getSizeModifierFromSizeCategory(SizeCategories category) const;
            public:
                CombatManeuvers(CharacterDescription &description, AbilityScores &abilityScores, BaseAttackBonus &baseAttackBonus);
                ~CombatManeuvers();

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string &observerName, Observer *observer) override;
                void unregisterObserver(const std::string &observerName) override;

                void addCMBModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled = true);
                void calculateTotalCombatManeuverBonus();
                int getCombatManeuverBonus() const;

                void addCMDModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled = true);
                void calculateTotalCombatManeuverDefense();
                int getCombatManeuverDefense() const;
        };
    }
}
#endif
