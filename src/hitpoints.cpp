#include <hitpoints.h>

namespace RulesEngine
{
    namespace Character
    {
        HitPoints::HitPoints(CharacterDescription& charDesc, AbilityScores& abilityScores)
        :
            abilityScores(abilityScores),
            characterDescription(charDesc)
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

            if (fieldName == "constitution") {
                generateHitPoints();
                return;
            }

            if (fieldName == "negativeLevels") {
                generateHitPoints();
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

        void HitPoints::resetMaxHPIfCharacterClassRemoved(const std::unordered_map<std::string, CharacterClass>& characterClasses)
        {
            //Check our die roll map to see if we are still retaining any class entries that aren't in the collection
            //returned from characterDescription.getClasses(). If we find one, invalidate all the die rolls because we've
            //made a pretty big change to the character
            for (auto& hpDieRollClassEntry : hpDieRollsByClass) {
                auto& className = hpDieRollClassEntry.first;
                auto charClassIterator = characterClasses.find(className);

                if (charClassIterator == characterClasses.end()) {
                    hpDieRollsByClass.clear();
                    firstHitDieRollCalculated = false;
                    return;
                }
            }
        }

        void HitPoints::setMaxHPFromDieRolls(const std::unordered_map<std::string, std::vector<unsigned int>>& hpDieRollsByClass)
        {
            int maxHP = 0;
            int conModifier = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::CON);

            for (auto& classRollPair : hpDieRollsByClass) {
                for (auto rollValue : classRollPair.second) {
                    int rollWithConMod = conModifier + static_cast<int>(rollValue);

                    //characters are always guaranteed at least 1hp per level up, even with a negative modifier
                    if (rollWithConMod <= 0) {
                        rollWithConMod = 1;
                    }

                    maxHP += rollWithConMod;
                }
            }

            //Negative level penalty on HP is a flat -5 points per negative level
            maxHP += static_cast<int>(characterDescription.getTotalNegativeLevels()) * -5;

            if (maxHP < 0) {
                maxHP = 0;
            }

            maxHitPoints = maxHP;
            if (currentHitPoints > maxHitPoints) {
                currentHitPoints = maxHitPoints;
            }
        }

        void HitPoints::generateHitPointsPFSRules()
        {
            //Similar to the HP generation for core rules, but instead of rolling
            //each level, the level grants (HD Size / 2) + 1 + conModifier.
            //Note: no class exists that I know of with <d6 hit die but, 
            //we let users kind of define their own classes without really enforcing rules. So, guarantee
            //every level grants at least 1 hp in the case we have like a -5 
            
            auto& characterClasses = characterDescription.getClasses();
            int conModifier = abilityScores.getTotalAbilityModifier(AbilityScoreTypes::CON);
            int maxHP = 0;
            bool firstLevelCalculated = false;

            for (auto& characterClass : characterClasses) {
                int hpToAdd = 0;

                if (firstLevelCalculated) {
                    hpToAdd = ((characterClass.second.hitDieSize / 2) + 1 + conModifier) * characterClass.second.classLevel;
                } else {
                    hpToAdd = ((characterClass.second.hitDieSize / 2) + 1 + conModifier) * (characterClass.second.classLevel - 1) + (characterClass.second.hitDieSize + conModifier);
                    firstLevelCalculated = true;
                }

                if (hpToAdd <= 0) {
                    hpToAdd = characterClass.second.classLevel;
                }

                maxHP += hpToAdd;
            }

            maxHP += static_cast<int>(characterDescription.getTotalNegativeLevels()) * -5;

            maxHitPoints = maxHP;
            if (currentHitPoints > maxHitPoints) {
                currentHitPoints = maxHitPoints;
            }
        }

        void HitPoints::generateHitPointsCoreRules()
        {
            auto& characterClasses = characterDescription.getClasses();
            std::random_device randomDevice;
            auto randomGenerator = std::mt19937(randomDevice());

            resetMaxHPIfCharacterClassRemoved(characterClasses);

            for (auto& characterClass : characterClasses) {
                auto& className = characterClass.second.className;
                auto hpDieRollsIterator = hpDieRollsByClass.find(className);

                if (hpDieRollsIterator == hpDieRollsByClass.end()) {

                    //add the class to our collection
                    hpDieRollsByClass[className] = std::vector<unsigned int>();

                    //Create a distrubtion from 1 - Die Max Size, inclusive
                    auto uniformDistribution = std::uniform_int_distribution<unsigned int>(1, characterClass.second.hitDieSize);

                    //fill it with the appropriate amount of randomly rolled hit die
                    for (unsigned int i = 0; i < characterClass.second.classLevel; ++i) {
                        if (firstHitDieRollCalculated == false) {
                            hpDieRollsByClass[className].push_back(characterClass.second.hitDieSize);
                            firstHitDieRollCalculated = true;
                            continue;
                        }

                        hpDieRollsByClass[className].push_back(uniformDistribution(randomGenerator));
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
                        for (unsigned int i = classDieRolls.size(); i < classLevel; ++i) {
                            classDieRolls.push_back(uniformDistribution(randomGenerator));
                        }
                    } else {
                        classDieRolls.erase(
                            classDieRolls.begin() + classLevel,
                            classDieRolls.end()
                        );
                    }
                }
            }

            //Now, update new max hit points value
            setMaxHPFromDieRolls(hpDieRollsByClass);
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

        const std::unordered_map<std::string, std::vector<unsigned int>>& HitPoints::getHpDieRollsByClass() const
        {
            return hpDieRollsByClass;
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
            hpDieRollsByClass.clear();
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
