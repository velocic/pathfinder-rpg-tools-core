#include <hitpoints.h>

namespace RulesEngine
{
    namespace Character
    {
        void HitPoints::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void HitPoints::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //TODO: need to subscribe to negative levels (which needs to be added to characterdescription module)
            //TODO: need to subscribe to abilityscores for constitution score updates
        }

        void HitPoints::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void HitPoints::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void HitPoints::autoGenerateMaximumHitPoints()
        {
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
            autoGenerateMaximumHitPoints();
        }

        void HitPoints::setCurrentHitPoints(int currentHP)
        {
            currentHitPoints = currentHP;
        }

        void HitPoints::setMaximumHitPoints(int maxHP)
        {
            maxHitPoints = maxHP;
        }
    }
}
