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

        void HitPoints::addTemporaryHitPoints(unsigned int tempHP)
        {
        }

        void HitPoints::dealNonLethalDamage(unsigned int damageAmount)
        {
        }

        void HitPoints::dealDamage(unsigned int damageAmount)
        {
        }

        void HitPoints::healHitPoints(unsigned int healAmount)
        {
        }

        int HitPoints::getCurrentHitPoints() const
        {
            return currentHitPoints;
        }

        unsigned int HitPoints::getNonLethalDamage() const
        {
            return nonLethalDamage;
        }

        unsigned int HitPoints::getMaxHitPoints() const
        {
            return maxHitPoints;
        }

        unsigned int HitPoints::getTemporaryHitPoints() const
        {
            return temporaryHitPoints;
        }

        void HitPoints::reset()
        {
        }

        void HitPoints::resetNonLethalDamage()
        {
        }

        void HitPoints::resetTemporaryHitPoints()
        {
        }

        void HitPoints::resetTotalHitPoints()
        {
        }

        void HitPoints::setCurrentHitPoints(int currentHP)
        {
        }

        void HitPoints::setMaximumHitPoints(unsigned int maxHP)
        {
        }
    }
}
