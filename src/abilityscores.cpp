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

        // void updateBaseAbilityModifier(AbilityScoreTypes ability);
        // void updateTotalTempAbilityScoreAdjustment(AbilityScoreTypes ability);
        // void updateTotalTempAbilityScoreModifier(AbilityScoreTypes ability);
        //
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
