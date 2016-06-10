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

        //Dependent on baseScore & totalTempAdjustment being up to date
        void AbilityScores::updateTotalAbilityScore(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;
            targetAbility.totalScore = baseScore + totalTempAdjustment;
        }

        //Dependent on totalScore & its' dependencies
        void AbilityScores::updateBaseAbilityModifier(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;

            if (targetAbility.baseScore <= 0) {
                if (ability == AbilityScoreTypes::STR || ability == AbilityScoreTypes::WIS || ability == AbilityScoreTypes::CHA) {
                    targetAbility.baseModifier = static_cast<int>(SpecialAbilityScoreValues::Unconscious);
                } else if (ability == AbilityScoreTypes::DEX) {
                    targetAbility.baseModifier = static_cast<int>(SpecialAbilityScoreValues::Immobile);
                } else if (ability == AbilityScoreTypes::INT) {
                    targetAbility.baseModifier = static_cast<int>(SpecialAbilityScoreValues::Comatose);
                } else {
                    //AbilityScoreTypes::CON
                    targetAbility.baseModifier = static_cast<int>(SpecialAbilityScoreValues::Dead);
                }
            } else {
                targetAbility.baseModifier = (targetAbility.baseScore - 10) / 2;
            }
        }

        //Dependent on target ability score's tempAdjustments being fully populated
        void AbilityScores::updateTotalTempAbilityScoreAdjustment(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;

            int totalTempAdjustment = 0;

            //Need to fix. Can't just sum them all. Most bonuses from same source type don't stack
            //May also need to revisit. Some items (i.e. int headband) give "Permanent" increases
            //once 24hr have passed. Which means some gear-based bonuses increase skills, hp, etc, while others
            //don't. Probably should solve that in the equipment section by having "Permanent" and "Temporary" modifiers
            //on gear be in separate categories
            for (auto& modifier : targetAbility.tempAdjustments) {
                totalTempAdjustment += modifier.second.modifierValue;
            }

            targetAbility.totalTempAdjustment = totalTempAdjustment;
        }

        //Dependent on baseScore and totalAbilityModifier
        void AbilityScores::updateTotalTempAbilityScoreModifier(AbilityScoreTypes ability)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability)).second;

            //Need to factor in temp score adjustment being odd and base score being odd. i.e.
            //Base STR: 15
            //Base STR Modifier: +2
            //Temp STR Bonus: 3
            //Temp STR Modifier: +2 (factors in the hanging odd str point from base to roll up temp bonus to a +2 instead
            //  of just +1

            int tempAdjustmentScore = 0;

            if (targetAbility.baseScore % 2 != 0) {
                tempAdjustmentScore = 1;
            }

            tempAdjustmentScore += targetAbility.totalTempAdjustment;

            targetAbility.totalTempModifier = tempAdjustmentScore / 2;
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
