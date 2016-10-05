#include <hitpoints.h>

namespace RulesEngine
{
    namespace Character
    {
        HitPoints::HitPoints(CharacterDescription& charDesc, AbilityScores& abilityScores)
        :
            characterDescription(charDesc),
            abilityScores(abilityScores)
        {
            characterDescription.registerObserver("hitpoints", this);
            this->abilityScores.registerObserver("hitpoints", this);
        }

        HitPoints::~HitPoints()
        {
            characterDescription.unregisterObserver("hitpoints");
            abilityScores.unregisterObserver("hitpoints");
        }

        void HitPoints::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void HitPoints::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            if (fieldName == "class") {
                generateHitPoints();
                return;
            }

            if (fieldName == "negativeLevels") {
                //total and current hp need to be reduced by 5 per neg level
                throw std::logic_error("Unimplemented");
                return;
            }

            if (fieldName == "constitution") {
                throw std::logic_error("Unimplemented");
                return;
            }
        }

        void HitPoints::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void HitPoints::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void HitPoints::generateHitPointsPFSRules()
        {
            //Almost exactly similar to the HP generation for core rules, but instead of rolling
            //each level, the level grants (HD Size / 2) + 1
            
            //NOTE: The very first level is always the full hit die amount
            throw std::logic_error("Unimplemented");
        }

        void HitPoints::generateHitPointsCoreRules()
        {
            //TODO: The very first level should be set to the full hit die amount

            auto& characterClasses = characterDescription.getClasses();
            std::random_device randomDevice;
            auto randomGenerator = std::mt19937(randomDevice());

            for (auto& characterClass : characterClasses) {
                auto& className = characterClass.second.className;
                auto hpDieRollsIterator = hpDieRollsByLevel.find(className);

                if (hpDieRollsIterator == hpDieRollsByLevel.end()) {

                    //add the class to our collection
                    hpDieRollsByLevel[className] = std::vector<unsigned int>();

                    //Create a distrubtion from 1 - Die Max Size, inclusive
                    auto uniformDistribution = std::uniform_int_distribution<unsigned int>(1, characterClass.second.hitDieSize);

                    //fill it with the appropriate amount of randomly rolled hit die
                    for (int i = 0; i < characterClass.second.classLevel; ++i) {
                        hpDieRollsByLevel[className].push_back(uniformDistribution(randomGenerator));
                    }
                } else {
                    //Class entry exists. Add HP rolls if more levels exist than we have rolls for. If level is lower
                    //than the number of rolls, we need to pop roll values starting from the end because levels were deleted
                    auto& classDieRolls = hpDieRollsIterator->second;
                    auto classLevel = characterClass.second.classLevel;

                    if (classDieRolls.size() == classLevel) {
                        continue;
                    }

                    //Create a distrubtion from 1 - Die Max Size, inclusive
                    auto uniformDistribution = std::uniform_int_distribution<unsigned int>(1, characterClass.second.hitDieSize);

                    if (classDieRolls.size() < classLevel) {
                        for (int i = classDieRolls.size(); i < classLevel; ++i) {
                            classDieRolls.push_back(uniformDistribution(randomGenerator));
                        }
                    } else {
                        classDieRolls.erase(
                            classDieRolls.begin() + (classLevel - 1),
                            classDieRolls.end()
                        );
                    }
                }
            }

            //Now, update new max hit points value
            unsigned int maxHP = 0;
            for (auto& classRollPair : hpDieRollsByLevel) {
                for (auto rollValue : classRollPair.second) {
                    maxHP += rollValue;
                }
            }

            maxHitPoints = maxHP;
        }

        void HitPoints::generateHitPoints()
        {
            if (usePFSStyleFixedHPCalculation == true) {
                generateHitPointsPFSRules();
                return;
            }

            generateHitPointsCoreRules();
        }

        void HitPoints::addTemporaryHitPoints(int tempHP)
        {
            temporaryHitPoints += tempHP;
        }

        void HitPoints::dealNonLethalDamage(int damageAmount)
        {
            nonLethalDamage += damageAmount;

            //TODO: need to set a special status as UNCONSCIOUS if this is greater than maxHitPoints
            //architecturally, this is kind of tough since abilityscores already
            //has that representation. Maybe move it to characterdescription and have
            //both of these classes talk to it?
        }

        void HitPoints::dealDamage(int damageAmount)
        {
            auto actualDamageAmount = damageAmount;
            if (temporaryHitPoints > 0) {
                actualDamageAmount -= temporaryHitPoints;
                temporaryHitPoints -= damageAmount;

                if (temporaryHitPoints < 0) {
                    temporaryHitPoints = 0;
                }
            }

            currentHitPoints -= damageAmount;

            //TODO: need to set a special status as DEAD if this is less than 0
            //architecturally, this is kind of tough since abilityscores already
            //has that representation. Maybe move it to characterdescription and have
            //both of these classes talk to it?
        }

        void HitPoints::healHitPoints(int healAmount)
        {
            currentHitPoints += healAmount;
            nonLethalDamage -= healAmount;

            if (currentHitPoints > maxHitPoints) {
                currentHitPoints = maxHitPoints;
            }

            if (nonLethalDamage < 0) {
                nonLethalDamage = 0;
            }
        }

        int HitPoints::getCurrentHitPoints() const
        {
            return currentHitPoints;
        }

        int HitPoints::getNonLethalDamage() const
        {
            return nonLethalDamage;
        }

        int HitPoints::getMaxHitPoints() const
        {
            return maxHitPoints;
        }

        int HitPoints::getTemporaryHitPoints() const
        {
            return temporaryHitPoints;
        }

        void HitPoints::reset()
        {
            resetNonLethalDamage();
            resetTemporaryHitPoints();
            setCurrentHitPoints(getMaxHitPoints());
        }

        void HitPoints::resetNonLethalDamage()
        {
            nonLethalDamage = 0;
        }

        void HitPoints::resetTemporaryHitPoints()
        {
            temporaryHitPoints = 0;
        }

        void HitPoints::recalculateTotalHitPoints()
        {
            hpDieRollsByLevel.clear();
            generateHitPoints();
        }

        void HitPoints::setCurrentHitPoints(int currentHP)
        {
            currentHitPoints = currentHP;
        }

        void HitPoints::setMaximumHitPoints(int maxHP)
        {
            maxHitPoints = maxHP;
        }

        void HitPoints::setUsePFSStyleFixedHPCalculation(bool shouldUse)
        {
            usePFSStyleFixedHPCalculation = shouldUse;

            recalculateTotalHitPoints();
        }
    }
}
