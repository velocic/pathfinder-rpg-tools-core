#ifndef INITIATIVE_H
#define INITIATIVE_H

#include <string>
#include <unordered_map>

#include <abilityscores.h>
#include <observersubject.h>
#include <observer.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        struct InitiativeModifier
        {
            std::string description;
            std::string sourceName;
            int modifierValue = 0;
            bool enabled = false;
        };

        class PATHFINDER_RULES_ENGINE_EXPORT Initiative :
            public ObserverSubject,
            public Observer
        {
            private:
                AbilityScores& abilityScores;
                int totalInitiative = 0;

                std::unordered_map<std::string, InitiativeModifier> miscInitiativeModifiers;

                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
            public:
                Initiative(AbilityScores& abilityscores);
                ~Initiative();

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;
        };
    }
}

#endif
