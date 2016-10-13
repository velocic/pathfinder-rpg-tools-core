#include <initiative.h>

namespace RulesEngine
{
    namespace Character
    {
        Initiative::Initiative(AbilityScores& abilityScores)
        :
            abilityScores(abilityScores)
        {
            abilityScores.registerObserver("initiative", this);
        }

        Initiative::~Initiative()
        {
            abilityScores.unregisterObserver("initiative");
        }

        void Initiative::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void Initiative::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            if (fieldName == "dexterity") {
                //do dexterity logic
            }
        }

        void Initiative::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void Initiative::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }
    }
}
