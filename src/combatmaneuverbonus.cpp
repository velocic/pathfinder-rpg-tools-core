#include <combatmaneuverbonus.h>

namespace RulesEngine
{
    namespace Character
    {
        CombatManeuverBonus::CombatManeuverBonus(CharacterDescription& description, AbilityScores& abilityScores, BaseAttackBonus &baseAttackBonus)
        :
            characterDescription(description),
            abilityScores(abilityScores),
            baseAttackBonus(baseAttackBonus)
        {
            characterDescription.registerObserver("combatManeuverBonus", this);
            abilityScores.registerObserver("combatManeuverBonus", this);
            baseAttackBonus.registerObserver("combatManeuverBonus", this);

            calculateTotalCombatManeuverBonus();
        }

        CombatManeuverBonus::~CombatManeuverBonus()
        {
            characterDescription.unregisterObserver("combatManeuverBonus");
            abilityScores.unregisterObserver("combatManeuverBonus");
            baseAttackBonus.unregisterObserver("combatManeuverBonus");
        }

        void CombatManeuverBonus::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void CombatManeuverBonus::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            if (fieldName == "baseAttackBonus" || fieldName == "strength" || fieldName == "sizeCategory") {
                calculateTotalCombatManeuverBonus();
            }
        }

        void CombatManeuverBonus::registerObserver(const std::string &observerName, Observer *observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void CombatManeuverBonus::unregisterObserver(const std::string &observerName)
        {
            observers.erase(observerName);
        }

        void CombatManeuverBonus::addCMBModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled)
        {
            CMBModifiers[sourceName] = CombatManeuverModifier{type, description, sourceName, modifierValue, enabled};

            calculateTotalCombatManeuverBonus();
        }

        void CombatManeuverBonus::calculateTotalCombatManeuverBonus()
        {
            //CMB = BAB + STR mod + size mod + contributingCMBModifiers total
            //Size Mods: Fine -8, Diminutive -4, Tiny -2, Small -1, Medium +0, Large +1, Huge +2, Gargantuan +4, Colossal +8
            int totalContributingModifiers = 0;
            int sizeModifier = 0;

            auto contributingModifiers = getContributingCMBModifiers(CMBModifiers);
            auto totalBAB = baseAttackBonus.getTotalBaseAttackBonus();
            auto strMod = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::STR);
            auto sizeCategory = characterDescription.getSizeCategory();

            for (const auto& contributingModifier : contributingModifiers) {
                totalContributingModifiers += contributingModifier.second.modifierValue;
            }

            //Translate size category into specific size modifier amount
            if (sizeCategory == SizeCategories::Fine) {
                sizeModifier = -8;
            } else if (sizeCategory == SizeCategories::Diminutive) {
                sizeModifier = -4;
            } else if (sizeCategory == SizeCategories::Tiny) {
                sizeModifier = -2;
            } else if (sizeCategory == SizeCategories::Small) {
                sizeModifier = -1;
            } else if (sizeCategory == SizeCategories::Medium) {
                sizeModifier = 0;
            } else if (sizeCategory == SizeCategories::Large) {
                sizeModifier = 1;
            } else if (sizeCategory == SizeCategories::Huge) {
                sizeModifier = 2;
            } else if (sizeCategory == SizeCategories::Gargantuan) {
                sizeModifier = 4;
            } else {
                sizeModifier = 8;
            }

            totalCombatManeuverBonus = totalBAB + strMod + sizeModifier + totalContributingModifiers;

            notifyObservers("combatManeuverBonus");
        }

        int CombatManeuverBonus::getCombatManeuverBonus() const
        {
            return totalCombatManeuverBonus;
        }

        std::unordered_map<std::string, CombatManeuverModifier> CombatManeuverBonus::getContributingCMBModifiers(const std::unordered_map<std::string, CombatManeuverModifier>& rawModifierList) const
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
    }
}
