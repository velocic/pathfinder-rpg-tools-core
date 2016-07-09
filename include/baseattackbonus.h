#ifndef BASEATTACKBONUS_H
#define BASEATTACKBONUS_H

#include <string>
#include <unordered_map>
#include <vector>

#include <characterdescription.h>
#include <observersubject.h>
#include <observer.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        class PATHFINDER_RULES_ENGINE_EXPORT BaseAttackBonus :
            public ObserverSubject,
            public Observer
        {
            private:
                unsigned int totalBaseAttackBonus = 0;
                std::vector<std::pair<std::string, unsigned int>> baseAttackBonusByClass;
                std::unordered_map<std::string, Observer*> observers;

                void calculateBaseAttackBonus(const CharacterDescription& characterDescription);
                void notifyObservers(const std::string& fieldName) override;
            public:
                BaseAttackBonus() {}

                //Unregister from modules we're observing
                ~BaseAttackBonus() {}

                unsigned int getTotalBaseAttackBonus() const;
                const std::vector<std::pair<std::string, unsigned int>>& getBaseAttackBonusByClass() const;
                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;
        };
    }
}

#endif
