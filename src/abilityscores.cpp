#include <abilityscores.h>

namespace RulesEngine
{
    namespace Character
    {
        AbilityScores::AbilityScores()
        {
            AbilityScore emptyAbilityScore;

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::STR),
                    emptyAbilityScore
                )
            );

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::DEX),
                    emptyAbilityScore
                )
            );

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::CON),
                    emptyAbilityScore
                )
            );

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::INT),
                    emptyAbilityScore
                )
            );

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::WIS),
                    emptyAbilityScore
                )
            );

            abilityScores.insert(
                std::make_pair(
                    static_cast<int>(AbilityScoreTypes::CHA),
                    emptyAbilityScore
                )
            );
        }

        void AbilityScores::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        //Dependent on baseScore & totalAdjustment being up to date
        void AbilityScores::calculateTotalAbilityScore(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;
            targetAbility.totalScore = targetAbility.baseScore + targetAbility.totalAdjustment;
        }

        //Dependent on baseScore
        void AbilityScores::calculateBaseAbilityModifier(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;
            targetAbility.baseModifier = (targetAbility.baseScore - 10) / 2;
        }

        SpecialAbilityScoreValues AbilityScores::determineCharacterStatus(AbilityScoreTypes ability, int damageValue)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            if ((targetAbility.baseScoreWithPermanentAdjustments - damageValue) > 0) {
                return SpecialAbilityScoreValues::Normal;
            }

            if (ability == AbilityScoreTypes::STR || ability == AbilityScoreTypes::WIS || ability == AbilityScoreTypes::CHA) {
                return SpecialAbilityScoreValues::Unconscious;
            } else if (ability == AbilityScoreTypes::DEX) {
                return SpecialAbilityScoreValues::Immobile;
            } else if (ability == AbilityScoreTypes::INT) {
                return SpecialAbilityScoreValues::Comatose;
            } else {
                //AbilityScoreTypes::CON
                return SpecialAbilityScoreValues::Dead;
            }
        }

        //Dependent on baseScoreWithPermanentAdjusments being up-to-date
        void AbilityScores::calculateBaseModifierWithPermanentAdjustments(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.baseModifierWithPermanentAdjustments = (targetAbility.baseScoreWithPermanentAdjustments - 10) / 2;

            targetAbility.characterStatus = determineCharacterStatus(ability, 0);
        }

        //Dependent on target ability score's tempAdjustments & permanentAdjustments being fully populated
        void AbilityScores::calculateTotalAbilityScoreAdjustment(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            std::unordered_map<std::string, AbilityScoreBonus> contributingTemporaryBonuses = getContributingBonusesFromRawBonusList(targetAbility.tempAdjustments);
            std::unordered_map<std::string, AbilityScoreBonus> contributingBonuses = getContributingBonusesFromRawBonusList(contributingTemporaryBonuses, targetAbility.permanentAdjustments);

            std::vector<AbilityScoreBonus> flattenedContributingBonuses;

            int totalAdjustment = 0;

            for (auto& bonus : contributingBonuses) {
                flattenedContributingBonuses.push_back(bonus.second);
                totalAdjustment += bonus.second.modifierValue;
            }

            targetAbility.contributingAdjustments = flattenedContributingBonuses;

            targetAbility.totalAdjustment = totalAdjustment;
        }

        void AbilityScores::calculateTotalAbilityScoreDamage(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            int totalDamage = 0;

            for (auto& damage : targetAbility.abilityDamage) {
                totalDamage += damage.second.modifierValue;
            }

            targetAbility.totalAbilityDamage = totalDamage;
            targetAbility.characterStatus = determineCharacterStatus(ability, totalDamage);
        }

        void AbilityScores::calculateTotalAbilityScoreDrain(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            int totalDrain = 0;

            for (auto& drain : targetAbility.abilityPenalties) {
                totalDrain += drain.second.modifierValue;
            }

            targetAbility.totalAbilityDrain = totalDrain;
            targetAbility.characterStatus = determineCharacterStatus(ability, totalDrain);
        }

        void AbilityScores::calculateTotalAbilityScorePenalties(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            int totalPenalty = 0;

            for (auto& penalty : targetAbility.abilityPenalties) {
                totalPenalty += penalty.second.modifierValue;
            }

            //Penalties can never reduce a stat below 1, otherwise follows same rules as Ability Damage
            if ((targetAbility.baseScoreWithPermanentAdjustments - totalPenalty) < 1) {
                totalPenalty = targetAbility.baseScoreWithPermanentAdjustments - 1;
            }

            targetAbility.totalAbilityPenalty = totalPenalty;
        }


        //Dependent on totalScore
        void AbilityScores::calculateTotalAbilityScoreModifier(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.totalAbilityModifier = targetAbility.totalScore / 2;
        }

        void AbilityScores::calculateBaseScoreWithPermanentAdjustments(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;
            std::unordered_map<std::string, AbilityScoreBonus> bonuses = getContributingBonusesFromRawBonusList(targetAbility.permanentAdjustments);

            int totalBonus = 0;

            for (auto& bonus : bonuses) {
                totalBonus += bonus.second.modifierValue;
            }

            targetAbility.baseScoreWithPermanentAdjustments = totalBonus + targetAbility.baseScore - targetAbility.totalAbilityDrain;
            targetAbility.characterStatus = determineCharacterStatus(ability, 0);
        }

        std::unordered_map<std::string, AbilityScoreBonus> AbilityScores::getContributingBonusesFromRawBonusList(const std::unordered_map<std::string, AbilityScoreBonus>& mergeList, const std::unordered_map<std::string, AbilityScoreBonus>& rawBonusList)
        {
            std::unordered_map<std::string, AbilityScoreBonus> contributingBonuses = mergeList;

            //For each registered bonus
            for (auto& bonus : rawBonusList) {
                auto& adjustment = bonus.second;

                //Untyped bonuses always stack, so add them blindly
                if (adjustment.modifierType == AbilityScoreModifiers::Untyped) {
                    contributingBonuses.insert(bonus);
                    continue;
                }

                //Search to see if we're already factoring in a bonus with the same type as the current adjustment
                auto contributingBonusesIterator = std::find_if(
                    contributingBonuses.begin(),
                    contributingBonuses.end(),
                    [&](const std::pair<std::string, AbilityScoreBonus>& contributingBonus){
                        return contributingBonus.second.modifierType == adjustment.modifierType;
                    }
                );

                //Didn't find a bonus with the same type as the current one,
                //so add current bonus to the contributingBonuses list
                if (contributingBonusesIterator == contributingBonuses.end()) {
                    contributingBonuses.insert(bonus);
                } else { 
                    //Found a bonus that the current one can't stack with.
                    //Replace it if the current bonus is greater than the existing one. 
                    if (adjustment.modifierValue > contributingBonusesIterator->second.modifierValue) {
                        contributingBonuses.erase(contributingBonusesIterator);
                        contributingBonuses.insert(bonus);
                    }
                }
            }

            return contributingBonuses;
        }

        std::unordered_map<std::string, AbilityScoreBonus> AbilityScores::getContributingBonusesFromRawBonusList(const std::unordered_map<std::string, AbilityScoreBonus>& rawBonusList)
        {
            return getContributingBonusesFromRawBonusList(std::unordered_map<std::string, AbilityScoreBonus>(), rawBonusList);
        }
        
        // void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
        // void registerObserver(const std::string& observerName, Observer* observer) override;
        // void unregisterObserver(const std::string& observerName) override;
        //
        // void addAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
        // void addAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
        // void addAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
        // void addAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description);
        //
        // void removeAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName);
        // void removeAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName);
        // void removeAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName);
        // void removeAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName);
        //
        // void setBaseAbilityScore(AbilityScoreTypes ability, int baseScore);
    }
}
