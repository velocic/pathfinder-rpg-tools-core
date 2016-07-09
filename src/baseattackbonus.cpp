#include <baseattackbonus.h>

namespace RulesEngine
{
    namespace Character
    {
        void BaseAttackBonus::calculateBaseAttackBonus()
        {
        }

        void BaseAttackBonus::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        unsigned int BaseAttackBonus::getTotalBaseAttackBonus()
        {
            //Stubbed for now
            return 0;
        }

        void BaseAttackBonus::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //Needs to subscribe to updates about class levels (from any number of classes, with
            //  any combination of BAB progression types) from the characterdescription module
        }

        void BaseAttackBonus::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void BaseAttackBonus::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }
    }
}
