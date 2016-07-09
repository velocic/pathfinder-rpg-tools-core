#ifndef BASEATTACKBONUS_H
#define BASEATTACKBONUS_H

#include <unordered_map>

#include <observersubject.h>
#include <observer.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        class PATHFINDER_RULES_ENGINE_EXPORT BaseAttackBonus :
            ObserverSubject,
            Observer
        {
            private:
                unsigned int totalBaseAttackBonus = 0;
                std::unordered_map<std::string, Observer*> observers;

                void calculateBaseAttackBonus();
                void notifyObservers(const std::string& fieldName) override;
            public:
                BaseAttackBonus() {}

                //Unregister from modules we're observing
                ~BaseAttackBonus() {}

                unsigned int getTotalBaseAttackBonus();
                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;
        };
    }
}

#endif
