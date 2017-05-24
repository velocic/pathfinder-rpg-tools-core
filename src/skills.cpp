#include <skills.h>

namespace RulesEngine
{
    namespace Character
    {
        Skills::Skills(CharacterDescription& charDescription, AbilityScores& abilityScores)
        :
            characterDescription(charDescription),
            abilityScores(abilityScores)
        {
            skills[SkillType::Acrobatics] = Skill();
            skills[SkillType::Appraise] = Skill();
            skills[SkillType::Bluff] = Skill();
            skills[SkillType::Climb] = Skill();
            skills[SkillType::Craft] = Skill();
            skills[SkillType::Diplomacy] = Skill();
            skills[SkillType::DisableDevice] = Skill();
            skills[SkillType::Disguise] = Skill();
            skills[SkillType::EscapeArtist] = Skill();
            skills[SkillType::Fly] = Skill();
            skills[SkillType::HandleAnimal] = Skill();
            skills[SkillType::Heal] = Skill();
            skills[SkillType::Intimidate] = Skill();
            skills[SkillType::KnowledgeArcana] = Skill();
            skills[SkillType::KnowledgeDungeoneering] = Skill();
            skills[SkillType::KnowledgeEngineering] = Skill();
            skills[SkillType::KnowledgeGeography] = Skill();
            skills[SkillType::KnowledgeHistory] = Skill();
            skills[SkillType::KnowledgeLocal] = Skill();
            skills[SkillType::KnowledgeNature] = Skill();
            skills[SkillType::KnowledgeNobility] = Skill();
            skills[SkillType::KnowledgePlanes] = Skill();
            skills[SkillType::KnowledgeReligion] = Skill();
            skills[SkillType::Linguistics] = Skill();
            skills[SkillType::Perception] = Skill();
            skills[SkillType::Perform] = Skill();
            skills[SkillType::Profession] = Skill();
            skills[SkillType::Ride] = Skill();
            skills[SkillType::SenseMotive] = Skill();
            skills[SkillType::SleightOfHand] = Skill();
            skills[SkillType::Spellcraft] = Skill();
            skills[SkillType::Stealth] = Skill();
            skills[SkillType::Survival] = Skill();
            skills[SkillType::Swim] = Skill();
            skills[SkillType::UseMagicDevice] = Skill();

            characterDescription.registerObserver("skills", this);
            abilityScores.registerObserver("skills", this);

            calculateTotalSkillPoints();
        }

        Skills::~Skills()
        {
            characterDescription.unregisterObserver("skills");
            abilityScores.unregisterObserver("skills");
        }

        void Skills::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void Skills::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //TODO: listen for updates on fields we care about
        }

        void Skills::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void Skills::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void Skills::addGlobalSkillModifier(SkillModifierType modType, const std::string& description, const std::string& sourceName, int modValue, bool enabled)
        {
            globalSkillModifiers[sourceName] = SkillModifier{
                modType,
                description,
                sourceName,
                modValue,
                enabled
            };
        }

        void Skills::addSkillRanks(SkillType skill, int numRanks)
        {
            auto mapIterator = skills.find(skill);

            if (mapIterator != skills.end()) {
                mapIterator->second.ranks += numRanks;
            }

            calculateTotalSkillPoints();
        }

        void Skills::addSkillModifier(SkillType skill, SkillModifierType modType, const std::string& description, const std::string& sourceName, int modValue, bool enabled)
        {
            auto mapIterator = skills.find(skill);
            
            if (mapIterator != skills.end()) {
                auto& skillData = mapIterator->second;

                auto newModifier = SkillModifier{
                    modType,
                    description,
                    sourceName,
                    modValue,
                    enabled
                };

                skillData.modifiers[sourceName] = newModifier;
            }

            calculateTotalSkillPoints();
        }

        std::map<SkillType, int> Skills::getAllTotalSkillPoints() const
        {
            std::map<SkillType, int> totalSkillPoints;

            for (const auto& skillPair : skills) {
                const auto& skill = skillPair.second;

                auto skillTotal = std::make_pair(skillPair.first, skill.ranks);

                for (const auto& modifier : skill.modifiers) {
                    if (modifier.second.enabled == true) {
                        skillTotal.second += modifier.second.modifierValue;
                    }
                }

                for (const auto& modifier : globalSkillModifiers) {
                    if (modifier.second.enabled == true) {
                        skillTotal.second += modifier.second.modifierValue;
                    }
                }

                totalSkillPoints[skillTotal.first] = skillTotal.second;
            }

            return totalSkillPoints;
        }

        const std::unordered_map<std::string, SkillModifier>& Skills::getGlobalSkillModifiers() const
        {
            return globalSkillModifiers;
        }

        const std::unordered_map<std::string, SkillModifier>& Skills::getSkillModifiers(SkillType skill) const
        {
            const auto& mapIterator = skills.find(skill);

            return mapIterator->second.modifiers;
        }

        int Skills::getTotalSkillPoints(SkillType skill) const
        {
            const auto& mapIterator = skills.find(skill);

            int totalSkillPoints = 0;

            if (mapIterator == skills.end()) {
                return totalSkillPoints;
            }

            totalSkillPoints = mapIterator->second.ranks;

            for (const auto& skillModifier : mapIterator->second.modifiers) {
                if (skillModifier.second.enabled == true) {
                    totalSkillPoints += skillModifier.second.modifierValue;
                }
            }

            for (const auto& skillModifier : globalSkillModifiers) {
                if (skillModifier.second.enabled == true) {
                    totalSkillPoints += skillModifier.second.modifierValue;
                }
            }
        }

        void Skills::calculateTotalSkillPoints()
        {
        }

        void Skills::removeGlobalSkillModifier(const std::string& sourceName)
        {
            globalSkillModifiers.erase(sourceName);
        }

        void Skills::removeSkillRanks(SkillType skill, int numRanks)
        {
            auto mapIterator = skills.find(skill);

            if (mapIterator == skills.end()) {
                return;
            }

            mapIterator->second.ranks -= numRanks;

            calculateTotalSkillPoints();
        }

        void Skills::removeSkillModifier(SkillType skill, const std::string& sourceName)
        {
            auto skillMapIterator = skills.find(skill);

            if (skillMapIterator != skills.end()) {
                skillMapIterator->second.modifiers.erase(sourceName);
            }

            calculateTotalSkillPoints();
        }
    }
}
