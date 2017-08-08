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

            skills[SkillType::Acrobatics].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::Appraise].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::Bluff].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::Climb].keyAbility = Character::AbilityScoreTypes::STR;
            skills[SkillType::Craft].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::Diplomacy].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::DisableDevice].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::Disguise].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::EscapeArtist].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::Fly].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::HandleAnimal].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::Heal].keyAbility = Character::AbilityScoreTypes::WIS;
            skills[SkillType::Intimidate].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::KnowledgeArcana].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeDungeoneering].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeEngineering].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeGeography].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeHistory].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeLocal].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeNature].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeNobility].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgePlanes].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::KnowledgeReligion].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::Linguistics].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::Perception].keyAbility = Character::AbilityScoreTypes::WIS;
            skills[SkillType::Perform].keyAbility = Character::AbilityScoreTypes::CHA;
            skills[SkillType::Profession].keyAbility = Character::AbilityScoreTypes::WIS;
            skills[SkillType::Ride].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::SenseMotive].keyAbility = Character::AbilityScoreTypes::WIS;
            skills[SkillType::SleightOfHand].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::Spellcraft].keyAbility = Character::AbilityScoreTypes::INT;
            skills[SkillType::Stealth].keyAbility = Character::AbilityScoreTypes::DEX;
            skills[SkillType::Survival].keyAbility = Character::AbilityScoreTypes::WIS;
            skills[SkillType::Swim].keyAbility = Character::AbilityScoreTypes::STR;
            skills[SkillType::UseMagicDevice].keyAbility = Character::AbilityScoreTypes::CHA;

            characterDescription.registerObserver("skills", this);
            abilityScores.registerObserver("skills", this);

            calculateTotalSpendableRanks(abilityScores, characterDescription);
            calculateTotalSkillPoints(abilityScores, characterDescription);
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
            // if (fieldName == "strength") {
            // } else if (fieldName == "dexterity") {
            // } else if (fieldName == "constitution") {
            // } else if (fieldName == "intelligence") {
            // } else if (fieldName == "wisdom") {
            // } else if (fieldName == "charisma") {
            // } else if (fieldName == "class") {
            // }

            calculateTotalSpendableRanks(abilityScores, characterDescription);
            calculateTotalSkillPoints(abilityScores, characterDescription);
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

            calculateTotalSkillPoints(abilityScores, characterDescription);
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

            calculateTotalSkillPoints(abilityScores, characterDescription);
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

            return totalSkillPoints;
        }

        void Skills::calculateTotalSkillPoints(const AbilityScores& abilityScores, const CharacterDescription& charDescription)
        {
            for (const auto& skill : skills) {
                int skillTotal = skill.second.ranks;

                const auto& classes = charDescription.getClasses();

                bool isClassSkill = false;
                for (const auto& charClass : classes) {
                    for (const auto& classSkill : charClass.second.classSkills) {
                        if (classSkill == skill.second.skillType) {
                            isClassSkill = true;
                            break;
                        }
                    }
                    if (isClassSkill == true) {
                        break;
                    }
                }

                if (skill.second.ranks > 0 && isClassSkill == true) {
                    skillTotal += 3;
                }

                for (const auto& modifier : globalSkillModifiers) {
                    if (modifier.second.enabled == true) {
                        skillTotal += modifier.second.modifierValue;
                    }
                }

                for (const auto& modifier : skill.second.modifiers) {
                    if (modifier.second.enabled == true) {
                        skillTotal += modifier.second.modifierValue;
                    }
                }

                skillTotal += abilityScores.getTotalAbilityModifier(skill.second.keyAbility);

                totalSkillPoints[skill.second.skillType] = skillTotal;
            }
        }

        void Skills::calculateTotalSpendableRanks(const AbilityScores& abilityScores, const CharacterDescription& charDescription)
        {
            //Each level in a class must grant at minimum 1 skill point regardless of negative int mod
            //TODO: Consider other things that grant extra points, like Favored Class Bonus, or being a human.
            int totalRanks = 0;
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

            calculateTotalSkillPoints(abilityScores, characterDescription);
        }

        void Skills::removeSkillModifier(SkillType skill, const std::string& sourceName)
        {
            auto skillMapIterator = skills.find(skill);

            if (skillMapIterator != skills.end()) {
                skillMapIterator->second.modifiers.erase(sourceName);
            }

            calculateTotalSkillPoints(abilityScores, characterDescription);
        }
    }
}
