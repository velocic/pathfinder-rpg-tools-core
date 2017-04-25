#ifndef SKILLS_H
#define SKILLS_H

namespace RulesEngine
{
    namespace Character
    {
        enum class SkillType : int
        {
            Acrobatics,
            Appraise,
            Bluff,
            Climb,
            Craft, //Can have multiple subtypes
            Diplomacy,
            DisableDevice,
            Disguise,
            EscapeArtist,
            Fly,
            HandleAnimal,
            Heal,
            Intimidate,
            KnowledgeArcana,
            KnowledgeDungeoneering,
            KnowledgeEngineering,
            KnowledgeGeography,
            KnowledgeHistory,
            KnowledgeLocal,
            KnowledgeNature,
            KnowledgeNobility,
            KnowledgePlanes,
            KnowledgeReligion,
            Linguistics,
            Perception,
            Perform, //Can have multiple subtypes
            Profession, //Can have multiple subtypes
            Ride,
            SenseMotive,
            SleightOfHand,
            Spellcraft,
            Stealth,
            Survival,
            Swim,
            UseMagicDevice
        };

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

                std::vector<SkillModifier> globalSkillModifiers;
            public:
                Skills();
                ~Skills();
        };
    }
}

#endif
