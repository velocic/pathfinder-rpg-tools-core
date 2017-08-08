#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <map>
#include <vector>

#include <abilityscores.h>
#include <characterdescription.h>
#include <common.h>
#include <observer.h>
#include <observersubject.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        //TODO: Pare-down this list, hard to find a list of exactly what bonus types
        //can affect skills
        enum class SkillModifierType : int
        {
            Alchemical,
            Circumstance, //Stack with eachother
            Competence, //Don't stack
            Dodge, //Don't stack
            Enhancement, //Don't stack
            Inherent, //Stack
            Insight, //Don't stack
            Luck, //Don't stack
            Morale, //Don't stack
            Profane, //Don't stack
            Racial, 
            Sacred, //Don't stack
            Trait, //Don't stack
            Untyped //Stack
        };

        struct SkillModifier
        {
            SkillModifierType modifierType = SkillModifierType::Untyped;
            std::string description;
            std::string sourceName;
            int modifierValue = 0;
            bool enabled = false;
        };

        struct Skill
        {
            SkillType skillType;
            unsigned int ranks = 0;
            std::unordered_map<std::string, SkillModifier> modifiers;
            bool canUseUntrained = true;
            AbilityScoreTypes keyAbility;
        };

        class PATHFINDER_RULES_ENGINE_EXPORT Skills :
            public ObserverSubject,
            public Observer
        {
            private:
                CharacterDescription &characterDescription;
                AbilityScores &abilityScores;
                int totalSpendableRanks = 0;
                int spentRanks = 0;

                std::map<SkillType, Skill> skills;
                std::unordered_map<std::string, SkillModifier> globalSkillModifiers;
                std::map<SkillType, int> totalSkillPoints;

                std::unordered_map<std::string, Observer*> observers;

                void calculateTotalSpendableRanks(const AbilityScores& abilityScores, const CharacterDescription& charDescription);
                void notifyObservers(const std::string& fieldName) override;
            public:
                Skills(CharacterDescription& charDescription, AbilityScores& abilityScores);
                ~Skills();

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;

                void addGlobalSkillModifier(SkillModifierType modType, const std::string& description, const std::string& sourceName, int modValue, bool enabled = true);
                void addSkillRanks(SkillType skill, int numRanks);
                void addSkillModifier(SkillType skill, SkillModifierType modType, const std::string& description, const std::string& sourceName, int modValue, bool enabled = true);
                const std::unordered_map<std::string, SkillModifier>& getGlobalSkillModifiers() const;
                const std::unordered_map<std::string, SkillModifier>& getSkillModifiers(SkillType skill) const;
                int getTotalSkillPoints(SkillType skill) const;
                std::map<SkillType, int> getAllTotalSkillPoints() const;
                void calculateTotalSkillPoints(const AbilityScores& abilityScores, const CharacterDescription& charDescription);
                void removeGlobalSkillModifier(const std::string& sourceName);
                void removeSkillRanks(SkillType skill, int numRanks);
                void removeSkillModifier(SkillType skill, const std::string& sourceName);
        };
    }
}

#endif
