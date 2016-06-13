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

        //Dependent on baseScoreWithPermanentAdjusments being up-to-date
        void AbilityScores::calculateBaseModifierWithPermanentAdjustments(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            if (targetAbility.baseScoreWithPermanentAdjustments <= 0) {
                if (ability == AbilityScoreTypes::STR || ability == AbilityScoreTypes::WIS || ability == AbilityScoreTypes::CHA) {
                    targetAbility.baseModifierWithPermanentAdjustments = static_cast<int>(SpecialAbilityScoreValues::Unconscious);
                } else if (ability == AbilityScoreTypes::DEX) {
                    targetAbility.baseModifierWithPermanentAdjustments = static_cast<int>(SpecialAbilityScoreValues::Immobile);
                } else if (ability == AbilityScoreTypes::INT) {
                    targetAbility.baseModifierWithPermanentAdjustments = static_cast<int>(SpecialAbilityScoreValues::Comatose);
                } else {
                    //AbilityScoreTypes::CON
                    targetAbility.baseModifierWithPermanentAdjustments = static_cast<int>(SpecialAbilityScoreValues::Dead);
                }
            } else {
                targetAbility.baseModifierWithPermanentAdjustments = (targetAbility.baseScoreWithPermanentAdjustments - 10) / 2;
            }
        }

        //Dependent on target ability score's tempAdjustments & permanentAdjustments being fully populated
        void AbilityScores::calculateTotalAbilityScoreAdjustment(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            std::vector<AbilityScoreBonus> contributingTemporaryBonuses = getContributingBonusesFromRawBonusList(targetAbility.temporaryAdjustments);
            std::vector<AbilityScoreBonus> contributingBonuses = getContributingBonusesFromRawBonusList(contributingTemporaryBonuses, targetAbility.permanentAdjustments);

            targetAbility.contributingAdjustments = contributingBonuses;

            int totalAdjustment = 0;

            for (auto& bonus : contributingBonuses) {
                totalAdjustment += bonus.modifierValue;
            }

            targetAbility.totalAdjustment = totalBonus;
        }

        //Dependent on totalScore
        void AbilityScores::calculateTotalAbilityScoreModifier(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;

            targetAbility.totalAbilityModifier = totalScore / 2;
        }

        void calculateBaseScoreWithPermanentAdjustments(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;
            std::vector<AbilityScoreBonus> bonuses = getContributingBonusesFromRawBonusList(targetAbility.permanentAdjustments);

            int total = 0;

            for (auto& bonus : bonuses) {
                total += bonus.modifierValue;
            }

            targetAbility.baseScoreWithPermanentAdjustments = total + targetAbility.baseScore;
        }

        std::vector<AbilityScoreBonus> getContributingBonusesFromRawBonusList(const std::vector<AbilityScoreBonus>& mergeList, const std::vector<AbilityScoreBonus>& rawBonusList)
        {
            std::vector<AbilityScoreBonus> contributingBonuses = mergeList;

            //For each registered bonus
            for (auto& adjustment : rawBonusList) {

                //Untyped bonuses always stack, so add them blindly
                if (adjustment.modifierType == AbilityScoreModifiers::Untyped) {
                    contributingBonuses.push_back(adjustment);
                    continue;
                }

                //Search to see if we're already factoring in a bonus with the same type as the current adjustment
                auto contributingBonusesIterator = std::find_if(
                    contributingBonuses.begin(),
                    contributingBonuses.end(),
                    [&](const AbilityScoreBonus& contributingBonus){
                        return contributingBonus.modifierType == adjustment.modifierType;
                    }
                );

                //Didn't find a bonus with the same type as the current one,
                //so add current bonus to the contributingBonuses list
                if (contributingBonusesIterator == contributingBonuses.end()) {
                    contributingBonuses.push_back(adjustment);
                } else { 
                    //Found a bonus that the current one can't stack with.
                    //Replace it if the current bonus is greater than the existing one. 
                    if (adjustment.modifierValue > contributingBonusesIterator->modifierValue) {
                        (*contributingBonusesIterator) = adjustment;
                    }
                }
            }

            return contributingBonuses;
        }

        std::vector<AbilityScoreBonus> getContributingBonusesFromRawBonusList(const std::vector<AbilityScoreBonus>& rawBonusList)
        {
            return getContributingBonusesFromRawBonusList(std::vector<AbilityScoreBonus>(), rawBonusList);
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
