#include <combatmaneuvers.h>

namespace RulesEngine
{
    namespace Character
    {
        CombatManeuvers::CombatManeuvers(CharacterDescription& description, AbilityScores& abilityScores, BaseAttackBonus &baseAttackBonus)
        :
            characterDescription(description),
            abilityScores(abilityScores),
            baseAttackBonus(baseAttackBonus)
        {
            characterDescription.registerObserver("combatManeuvers", this);
            abilityScores.registerObserver("combatManeuvers", this);
            baseAttackBonus.registerObserver("combatManeuvers", this);

            calculateTotalCombatManeuverBonus();
            calculateTotalCombatManeuverDefense();
        }

        CombatManeuvers::~CombatManeuvers()
        {
            characterDescription.unregisterObserver("combatManeuvers");
            abilityScores.unregisterObserver("combatManeuvers");
            baseAttackBonus.unregisterObserver("combatManeuvers");
        }

        void CombatManeuvers::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void CombatManeuvers::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            if (fieldName == "baseAttackBonus" || fieldName == "strength" || fieldName == "sizeCategory") {
                calculateTotalCombatManeuverBonus();
            }

            if (fieldName == "baseAttackBonus" || fieldName == "strength" || fieldName == "dexterity" || fieldName == "sizeCategory") {
                calculateTotalCombatManeuverDefense();
            }
        }

        void CombatManeuvers::registerObserver(const std::string &observerName, Observer *observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void CombatManeuvers::unregisterObserver(const std::string &observerName)
        {
            observers.erase(observerName);
        }

        void CombatManeuvers::addCMBModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled)
        {
            CMBModifiers[sourceName] = CombatManeuverModifier{type, description, sourceName, modifierValue, enabled};

            calculateTotalCombatManeuverBonus();
        }

        void CombatManeuvers::calculateTotalCombatManeuverBonus()
        {
            //CMB = BAB + STR mod + size mod + contributingCMBModifiers total
            //Size Mods: Fine -8, Diminutive -4, Tiny -2, Small -1, Medium +0, Large +1, Huge +2, Gargantuan +4, Colossal +8
            int totalContributingModifiers = 0;

            auto contributingModifiers = getContributingModifiers(CMBModifiers);
            auto totalBAB = baseAttackBonus.getTotalBaseAttackBonus();
            auto strMod = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::STR);
            auto sizeCategory = characterDescription.getSizeCategory();

            for (const auto& contributingModifier : contributingModifiers) {
                totalContributingModifiers += contributingModifier.second.modifierValue;
            }

            auto sizeModifier = getSizeModifierFromSizeCategory(sizeCategory);

            totalCombatManeuverBonus = totalBAB + strMod + sizeModifier + totalContributingModifiers;

            notifyObservers("combatManeuvers");
        }

        int CombatManeuvers::getCombatManeuverBonus() const
        {
            return totalCombatManeuverBonus;
        }

        std::unordered_map<std::string, CombatManeuverModifier> CombatManeuvers::getContributingModifiers(const std::unordered_map<std::string, CombatManeuverModifier>& rawModifierList) const
        {
            std::unordered_map<std::string, CombatManeuverModifier> contributingModifiers; 

            for (auto& modifierPair : rawModifierList) {
                if (modifierPair.second.enabled == false) {
                    continue;
                }

                auto& modifier = modifierPair.second;

                if (modifier.modifierType == CombatManeuverModifierType::Circumstance || modifier.modifierType == CombatManeuverModifierType::Untyped) {
                    //These always stack; Add them without further consideration
                    contributingModifiers.insert(modifierPair);
                    continue;
                }

                //Check to see if we're conflicting with an existing modifier
                auto contributingModifierIterator = std::find_if(
                    contributingModifiers.begin(),
                    contributingModifiers.end(),
                    [&](const std::pair<std::string, CombatManeuverModifier>& contributingBonus){
                        return contributingBonus.second.modifierType == modifier.modifierType;
                    }
                );

                if (contributingModifierIterator == contributingModifiers.end()) {
                    contributingModifiers.insert(modifierPair);
                } else {
                    //If we found a modifier we can't stack with, replace it if the new one is greater
                    if (modifier.modifierValue > contributingModifierIterator->second.modifierValue) {
                        contributingModifiers.erase(contributingModifierIterator);
                        contributingModifiers.insert(modifierPair);
                    }
                }
            }

            return contributingModifiers;
        }

        int CombatManeuvers::getSizeModifierFromSizeCategory(SizeCategories category) const
        {
            if (category == SizeCategories::Fine) {
                return -8;
            } else if (category == SizeCategories::Diminutive) {
                return -4;
            } else if (category == SizeCategories::Tiny) {
                return -2;
            } else if (category == SizeCategories::Small) {
                return -1;
            } else if (category == SizeCategories::Medium) {
                return 0;
            } else if (category == SizeCategories::Large) {
                return 1;
            } else if (category == SizeCategories::Huge) {
                return 2;
            } else if (category == SizeCategories::Gargantuan) {
                return 4;
            }

            return 8;
        }

        void CombatManeuvers::addCMDModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled)
        {
            CMDModifiers[sourceName] = CombatManeuverModifier{type, description, sourceName, modifierValue, enabled};

            calculateTotalCombatManeuverDefense();
        }

        void CombatManeuvers::calculateTotalCombatManeuverDefense()
        {
            //CMD = 10 + BAB + Str mod + Dex mod + size mod + misc bonuses
            int totalContributingModifiers = 0;

            auto contributingModifiers = getContributingModifiers(CMDModifiers);
            auto totalBAB = baseAttackBonus.getTotalBaseAttackBonus();
            auto strMod = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::STR);
            auto dexMod = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::DEX);
            auto sizeCategory = characterDescription.getSizeCategory();

            for (const auto& contributingModifier : contributingModifiers) {
                totalContributingModifiers += contributingModifier.second.modifierValue;
            }

            auto sizeModifier = getSizeModifierFromSizeCategory(sizeCategory);

            totalCombatManeuverDefense = 10 + totalBAB + strMod + dexMod + sizeModifier + totalContributingModifiers;

            notifyObservers("combatManeuvers");
        }

        int CombatManeuvers::getCombatManeuverDefense() const
        {
            return totalCombatManeuverDefense;
        }
    }
}
