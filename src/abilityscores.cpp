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
                //ignore damage entries marked disabled by the user
                if (damage.second.enabled == false) {
                    continue;
                }

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
                //ignore drain entries marked disabled by the user
                if (drain.second.enabled == false) {
                    continue;
                }

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
                //ignore penalty entries marked disabled by the user
                if (penalty.second.enabled == false) {
                    continue;
                }

                totalPenalty += penalty.second.modifierValue;
            }

            //Penalties can never reduce a stat below 1, otherwise follows same rules as Ability Damage
            if ((targetAbility.baseScoreWithPermanentAdjustments - totalPenalty) < 1) {
                totalPenalty = targetAbility.baseScoreWithPermanentAdjustments - 1;
            }

            targetAbility.totalAbilityPenalty = totalPenalty;
        }

        std::string AbilityScores::mapAbilityScoreEnumToString(AbilityScoreTypes ability)
        {
            switch (ability) {
                case AbilityScoreTypes::STR:
                    return "strength";
                case AbilityScoreTypes::DEX:
                    return "dexterity";
                case AbilityScoreTypes::CON:
                    return "constitution";
                case AbilityScoreTypes::INT:
                    return "intelligence";
                case AbilityScoreTypes::WIS:
                    return "wisdom";
                case AbilityScoreTypes::CHA:
                    return "charisma";
            }

            return "Unknown Ability Score Type";
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
                //Ignore bonuses disabled by the user
                if (bonus.second.enabled == false) {
                    continue;
                }

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
        
        void AbilityScores::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //Subscribe to updates here, & recalculate as needed
        }

        void AbilityScores::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void AbilityScores::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void AbilityScores::addTemporaryAbilityScoreBonus(AbilityScoreModifiers modifierType, AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            AbilityScoreBonus bonus;
            bonus.modifierType = modifierType;
            bonus.affectedScore = ability;
            bonus.description = description;
            bonus.sourceName = sourceName;
            bonus.modifierValue = modifierValue;
            bonus.enabled = true;

            targetAbility.tempAdjustments.insert(std::make_pair(sourceName, bonus));

            calculateTotalAbilityScoreAdjustment(ability);
            calculateTotalAbilityScore(ability);
            calculateTotalAbilityScoreModifier(ability);

            notifyObservers(mapAbilityScoreEnumToString(ability));
        }

        void AbilityScores::addPermanentAbilityScoreBonus(AbilityScoreModifiers modifierType, AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            AbilityScoreBonus bonus;
            bonus.modifierType = modifierType;
            bonus.affectedScore = ability;
            bonus.description = description;
            bonus.sourceName = sourceName;
            bonus.modifierValue = modifierValue;
            bonus.enabled = true;

            targetAbility.permanentAdjustments.insert(std::make_pair(sourceName, bonus));


            calculateTotalAbilityScoreAdjustment(ability);
            calculateTotalAbilityScoreDrain(ability);
            calculateBaseScoreWithPermanentAdjustments(ability);
            calculateBaseModifierWithPermanentAdjustments(ability);
            calculateTotalAbilityScore(ability);
            calculateTotalAbilityScoreModifier(ability);

            notifyObservers(mapAbilityScoreEnumToString(ability));
        }

        void AbilityScores::addAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            AbilityScoreDamage damage;
            damage.affectedScore = ability;
            damage.description = description;
            damage.sourceName = sourceName;
            damage.modifierValue = modifierValue;
            damage.enabled = true;

            targetAbility.abilityDamage.insert(std::make_pair(sourceName, damage));

            calculateTotalAbilityScoreDamage(ability);

            notifyObservers(mapAbilityScoreEnumToString(ability));
        }

        void AbilityScores::addAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            AbilityScoreDrain drain;
            drain.affectedScore = ability;
            drain.description = description;
            drain.sourceName = sourceName;
            drain.modifierValue = modifierValue;
            drain.enabled = true;

            targetAbility.abilityDrain.insert(std::make_pair(sourceName, drain));

            calculateTotalAbilityScoreDrain(ability);
            calculateBaseScoreWithPermanentAdjustments(ability);
            calculateBaseModifierWithPermanentAdjustments(ability);

            notifyObservers(mapAbilityScoreEnumToString(ability));
        }

        void AbilityScores::addAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName, int modifierValue, const std::string& description)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            AbilityScorePenalty penalty;
            penalty.affectedScore = ability;
            penalty.description = description;
            penalty.sourceName = sourceName;
            penalty.modifierValue = modifierValue;
            penalty.enabled = true;

            targetAbility.abilityPenalties.insert(std::make_pair(sourceName, penalty));

            calculateTotalAbilityScorePenalties(ability);

            notifyObservers(mapAbilityScoreEnumToString(ability));
        }

        bool AbilityScores::doesTemporaryAbilityScoreBonusSourceExist(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto tempAdjustmentIterator = targetAbility.tempAdjustments.find(sourceName);

            if (tempAdjustmentIterator == targetAbility.tempAdjustments.end()) {
                return false;
            }

            return true;
        }

        bool AbilityScores::doesPermanentAbilityScoreBonusSourceExist(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto permanentAdjustmentIterator = targetAbility.permanentAdjustments.find(sourceName);

            if (permanentAdjustmentIterator == targetAbility.permanentAdjustments.end()) {
                return false;
            }

            return true;
        }

        bool AbilityScores::doesAbilityScoreDamageSourceExist(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto damageIterator = targetAbility.abilityDamage.find(sourceName);

            if (damageIterator == targetAbility.abilityDamage.end()) {
                return false;
            }

            return true;
        }

        bool AbilityScores::doesAbilityScoreDrainSourceExist(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto drainIterator = targetAbility.abilityDrain.find(sourceName);

            if (drainIterator == targetAbility.abilityDrain.end()) {
                return false;
            }

            return true;
        }

        bool AbilityScores::doesAbilityScorePenaltySourceExist(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto penaltyIterator = targetAbility.abilityPenalties.find(sourceName);

            if (penaltyIterator == targetAbility.abilityPenalties.end()) {
                return false;
            }

            return true;
        }

        void AbilityScores::removeTemporaryAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.tempAdjustments.erase(sourceName);
        }

        void AbilityScores::removePermanentAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.permanentAdjustments.erase(sourceName);
        }

        void AbilityScores::removeAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.abilityDamage.erase(sourceName);
        }

        void AbilityScores::removeAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.abilityDrain.erase(sourceName);
        }

        void AbilityScores::removeAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.abilityPenalties.erase(sourceName);
        }

        void AbilityScores::setBaseAbilityScore(AbilityScoreTypes ability, int baseScore)
        {
            //Only configured by user, so simple to just set directly.
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            targetAbility.baseScore = baseScore;
        }

        void AbilityScores::toggleTemporaryAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto& tempBonus = targetAbility.tempAdjustments.find(sourceName)->second;

            if (tempBonus.enabled == true) {
                tempBonus.enabled = false;
            } else {
                tempBonus.enabled = true;
            }
        }

        void AbilityScores::togglePermanentAbilityScoreBonus(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto& permanentBonus = targetAbility.permanentAdjustments.find(sourceName)->second;

            if (permanentBonus.enabled == true) {
                permanentBonus.enabled = false;
            } else {
                permanentBonus.enabled = true;
            }
        }

        void AbilityScores::toggleAbilityScoreDamage(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto& abilityDamage = targetAbility.abilityDamage.find(sourceName)->second;

            if (abilityDamage.enabled == true) {
                abilityDamage.enabled = false;
            } else {
                abilityDamage.enabled = true;
            }
        }

        void AbilityScores::toggleAbilityScoreDrain(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto& abilityDrain = targetAbility.abilityDrain.find(sourceName)->second;

            if (abilityDrain.enabled == true) {
                abilityDrain.enabled = false;
            } else {
                abilityDrain.enabled = true;
            }
        }

        void AbilityScores::toggleAbilityScorePenalty(AbilityScoreTypes ability, const std::string& sourceName)
        {
            auto& targetAbility = abilityScores.find(static_cast<int>(ability))->second;

            auto& abilityPenalty = targetAbility.abilityPenalties.find(sourceName)->second;

            if (abilityPenalty.enabled == true) {
                abilityPenalty.enabled = false;
            } else {
                abilityPenalty.enabled = true;
            }
        }
    }
}
