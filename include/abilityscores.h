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
        enum class AbilityScoreTypes : int
        {
            STR,
            DEX,
            CON,
            INT,
            WIS,
            CHA
        };

        enum class SpecialAbilityScoreValues : int
        {
            Unconscious = -100, //If Strength, Wisdom, or Charisma falls to 0
            Immobile = -99, //If Dexterity falls to 0
            Dead = -98, //If Constitution falls to 0
            Comatose = -97, //If Intelligence falls to 0
        };

        struct AbilityScore
        {
            int baseScore; //Initial score selected & modified by user only
            int totalScore; //Should not be adjusted directly by user, derived from all modifiers
            int baseModifier; //Only derived from baseScore
            int totalAbilityModifier; //Should not be adjusted directly by user, derived from all modifiers
            int totalTempAdjustment; //Number of ability points added or subtracted to/from base score to derive total score
            int totalTempModifier; //Number of ability points added or subtracted to/from base modifier to derive total modifier
            std::unordered_map<std::string, AbilityScoreBonus> tempAdjustments;
            std::unordered_map<std::string, AbilityScoreDamage> abilityDamage;
            std::unordered_map<std::string, AbilityScoreDrain> abilityDrain;
            std::unordered_map<std::string, AbilityScorePenalty> abilityPenalties;
        };

        enum class AbilityScoreModifiers
        {
            Alchemical,
            Enhancement,
            Inherent,
            Morale,
            Size
        };

        struct AbilityScoreBonus
        {
            AbilityScoreTypes affectedScore;
            AbilityScoreModifiers modifierType
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
        struct AbilityScorePenalty
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
                std::unordered_map<int, AbilityScore> abilityScores;
                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
                void updateTotalAbilityScore(AbilityScoreTypes ability);
                void updateBaseAbilityModifier(AbilityScoreTypes ability);
                void updateTotalTempAbilityScoreAdjustment(AbilityScoreTypes ability);
                void updateTotalTempAbilityScoreModifier(AbilityScoreTypes ability);
            public:
                AbilityScores() {}

                //Unregister from modules we're observing
                ~AbilityScores() {}

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;

                void addAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
                void addAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
                void addAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
                void addAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);

                void removeAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName);
                void removeAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName);
                void removeAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName);
                void removeAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName);

                void setBaseAbilityScore(AbilityScoreTypes ability, int baseScore);
        };
    }
}

#endif
