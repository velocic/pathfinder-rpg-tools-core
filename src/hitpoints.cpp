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
            }

            if (fieldName == "negativeLevels") {
                //total and current hp need to be reduced by 5 per neg level
                throw std::logic_error("Unimplemented");
            }

            if (fieldName == "constitution") {
                throw std::logic_error("Unimplemented");
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
            //NOTE: The very first level is always the full hit die amount

            //A crude way to do this could be to get the whole collection of classes w/ levels
            //from character description, and go over it in a few passes.

            //Data representation:
            //have some sort of map of class names to vectors of die rolls in this class
            //First pass:
            //check that all classes listed are present in the map.
            //  -if not in map, but in the class list from chardescription, add them
            //  - if in the map but not in the class list (i.e. the class was deleted
            //       from chardescription), remove it from the map
            //Second pass:
            //iterate through each class entry in the map. check the vector size.
            //  -if the vector size is smaller than the number of class levels from
            //    chardescription, add more die rolls to our class->die rolls map
            //  -if the vector size is bigger than the number of class levels from
            //    chardescription (i.e. levels were removed after having previously added them)
            //    remove die rolls from the end of the vector
            throw std::logic_error("Unimplemented");
        }

        void HitPoints::generateHitPoints()
        {
            if (usePFSStyleFixedHPCalculation == true) {
                generateHitPointsPFSRules();
                return;
            }

            generateHitPointsCoreRules();

            //how to determine which class specifically leveled up?
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
            resetTotalHitPoints();
        }

        void HitPoints::resetNonLethalDamage()
        {
            nonLethalDamage = 0;
        }

        void HitPoints::resetTemporaryHitPoints()
        {
            temporaryHitPoints = 0;
        }

        void HitPoints::resetTotalHitPoints()
        {
            //TODO: need a way to remember user-defined MAXHP value,
            //and to re-use that instead of just recalculating with PFS rules
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

            resetTotalHitPoints();
        }
    }
}
