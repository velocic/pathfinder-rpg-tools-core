#ifndef SKILLS_H
#define SKILLS_H

#include <common.h>

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
            SkillModifierType modifierType = SkillModifierTypes::Untyped;
            std::string description;
            std::string sourceName;
            int modifierValue = 0;
            bool enabled = false;
        };

        struct Skill
        {
            SkillType skillType;
            unsigned int ranks;
            std::vector<SkillModifier> modifiers;
            bool canUseUntrained = true;
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

                void calculateTotalSpendableRanks(const characterDescription& charDescription);

                std::unordered_map<SkillType, Skill> skills;
                std::vector<SkillModifier> globalSkillModifiers;
                std::unordered_map<SkillType, int> totalSkillPoints;
            public:
                Skills(CharacterDescription& charDescription, AbilityScores& abilityScores);
                ~Skills();

                void addSkillRanks(SkillType skill, int numRanks);
                void addSkillModifier(SkillType skill, SkillModifierType modType, const std::string& description, const std::string& sourceName, int modValue, bool enabled = true);
                void calculateTotalSkillPoints();
                void removeSkillRanks(SkillType skill, int numRanks);
                void removeSkillModifier(SkillType skill, const std::string& );
        };
    }
}

#endif
