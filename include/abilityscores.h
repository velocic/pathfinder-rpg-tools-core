#ifndef ABILITYSCORES_H
#define ABILITYSCORES_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace RulesEngine
{
    namespace Character
    {
        enum class AbilityScoreTypes
        {
            STR,
            DEX,
            CON,
            INT,
            WIS,
            CHA
        };

        struct AbilityScore
        {
            int baseScore; //Initial score selected & modified by user only
            int totalScore; //Should not be adjusted directly by user, derived from all modifiers
            int baseModifier; //Only derived from baseScore
            int totalAbilityModifier; //Should not be adjusted directly by user, derived from all modifiers
            int totalTempAdjustment;
            int totalTempModifier;
            std::unordered_map<std::string, AbilityScoreBonus> tempAdjustments;
            std::unordered_map<std::string, AbilityScoreBonus> tempModifiers;
            std::unordered_map<std::string, AbilityScoreDamage> abilityDamage;
            std::unordered_map<std::string, AbilityScoreDrain> abilityDrain;
            std::unordered_map<std::string, AbilityScorePenalties> abilityPenalties;
        };

        struct AbilityScoreBonus
        {
            AbilityScoreTypes affectedScore;
            std::string sourceName;
            int modifierValue;
            std::string description;
        };

        struct AbilityScoreDamage
        {
            AbilityScoreTypes affectedScore;
            std::string sourceName;
            int modifierValue;
            std::string description;
        };

        struct AbilityScoreDrain
        {
            AbilityScoreTypes affectedScore;
            std::string sourceName;
            int modifierValue;
            std::string description;
        };

        //Like ability damage, but cannot cause you to fall unconscious or die,
        //cannot decrease ability score to less than 1
        struct AbilityScorePenalties
        {
            AbilityScoreTypes affectedScore;
            std::string sourceName;
            int modifierValue;
            std::string description;
        };

        class AbilityScores :
            ObserverSubject,
            Observer
        {
            private:
                AbilityScore strength;
                AbilityScore dexterity;
                AbilityScore constitution;
                AbilityScore intelligence;
                AbilityScore wisdom;
                AbilityScore charisma;
                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
            public:
                AbilityScores() {}
                ~AbilityScores() {}

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;
        };
    }
}

#endif
