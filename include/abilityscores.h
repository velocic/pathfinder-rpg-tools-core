#ifndef ABILITYSCORES_H
#define ABILITYSCORES_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <observersubject.h>
#include <observer.h>

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

        enum class AbilityScoreModifiers : int
        {
            Alchemical, //No stacking info. Assume doesn't stack
            Enhancement, //No stacking, only highest applies
            Inherent, //No stacking, only highest applies
            Morale, //Doesn't stack
            Size, //Only one should exist, as creatures can only be a single size
            Untyped //Stacks. Examples include class features like Dragon Disciple's strength bonus
        };

        struct AbilityScoreBonus
        {
            AbilityScoreModifiers modifierType;
            AbilityScoreTypes affectedScore;
            std::string description;
            std::string sourceName;
            int modifierValue;
            bool enabled;
        };

        //From d20pfsrd.com:
        //"For every 2 points of damage you take to a single ability, apply a -1
        //penalty to skills and statistics listed with the relevant ability. If the
        //amount of ability damage you have taken equals or exceeds your ability
        //score, you immediately fall unconscious until the damage is less than 
        //your ability score. The only exception to this is your Constitution score.
        //If the damage to your Constitution is equal to or greater than your
        //Constitution score, you die."
        struct AbilityScoreDamage
        {
            AbilityScoreTypes affectedScore;
            std::string description;
            std::string sourceName;
            int modifierValue;
            bool enabled;
        };

        //Actually reduces the ability score, like an inverse permanent bonus.
        //Stats derived from the ability score should be reduced (like skill ranks,
        //max HP, etc)
        struct AbilityScoreDrain
        {
            AbilityScoreTypes affectedScore;
            std::string description;
            std::string sourceName;
            int modifierValue;
            bool enabled;
        };

        //Like ability damage, but cannot cause you to fall unconscious or die,
        //cannot decrease ability score to less than 1
        struct AbilityScorePenalty
        {
            AbilityScoreTypes affectedScore;
            std::string description;
            std::string sourceName;
            int modifierValue;
            bool enabled;
        };

        struct AbilityScore
        {
            int baseScore; //Initial score selected & modified by user only
            int baseScoreWithPermanentAdjustments; //baseScore + totalPermanentAdjustment
            int totalScore; //Should not be adjusted directly by user, derived from all modifiers including temporary bonuses like spell effects

            int baseModifier; //Only derived from baseScore
            int baseModifierWithPermanentAdjustments; //derived from baseScoreWithPermanentBonuses
            int totalAbilityModifier; //Should not be adjusted directly by user, derived from all modifiers

            //Number of ability points add or subtracted to/from base score to derive
            //total score. Both temp & permanent bonuses are factored int, with
            //non-stacking bonuses filtered out
            int totalAdjustment; 

            std::unordered_map<std::string, AbilityScoreBonus> tempAdjustments;
            std::unordered_map<std::string, AbilityScoreBonus> permanentAdjustments;

            std::vector<AbilityScoreBonus> contributingAdjustments; //For display purposes. List of permanent & temporary bonuses w/ non-stacking bonuses filtered out

            std::unordered_map<std::string, AbilityScoreDamage> abilityDamage;
            std::unordered_map<std::string, AbilityScoreDrain> abilityDrain;
            std::unordered_map<std::string, AbilityScorePenalty> abilityPenalties;
        };

        class AbilityScores :
            ObserverSubject,
            Observer
        {
            private:
                std::unordered_map<int, AbilityScore> abilityScores;
                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
                void calculateTotalAbilityScore(AbilityScoreTypes ability);
                void calculateBaseAbilityModifier(AbilityScoreTypes ability);
                void calculateTotalAbilityScoreAdjustment(AbilityScoreTypes ability);
                void calculateTotalAbilityScoreModifier(AbilityScoreTypes ability);
                void calculateBaseScoreWithPermanentAdjustments(AbilityScoreTypes ability);
                void calculateBaseModifierWithPermanentAdjustments(AbilityScoreTypes ability);

                //Helper function to strip non-stacking bonuses
                std::vector<AbilityScoreBonus> getContributingBonusesFromRawBonusList(const std::vector<AbilityScoreBonus>& rawBonusList);
                std::vector<AbilityScoreBonus> getContributingBonusesFromRawBonusList(const std::vector<AbilityScoreBonus>& mergeList, const std::vector<AbilityScoreBonus>& rawBonusList);
            public:
                AbilityScores();

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
