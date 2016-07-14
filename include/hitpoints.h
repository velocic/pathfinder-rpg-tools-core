#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <unordered_map>

#include <observersubject.h>
#include <observer.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        class PATHFINDER_RULES_ENGINE_EXPORT HitPoints :
            public ObserverSubject,
            public Observer
        {
            private:
                int currentHitPoints = 0;
                int nonLethalDamage = 0;
                int maxHitPoints = 0;
                int temporaryHitPoints = 0;

                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
            public:
                HitPoints() {}
                ~HitPoints() {}

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;

                //Generate Max HP from PFS rules (if user doesn't decide to roll themselves)
                void autoGenerateMaximumHitPoints();

                void addTemporaryHitPoints(int tempHP);

                void dealNonLethalDamage(int damageAmount);
                void dealDamage(int damageAmount);
                void healHitPoints(int healAmount);

                int getCurrentHitPoints() const;
                int getNonLethalDamage() const;
                int getMaxHitPoints() const;
                int getTemporaryHitPoints() const;

                void reset();
                void resetNonLethalDamage();
                void resetTemporaryHitPoints();
                void resetTotalHitPoints();

                void setCurrentHitPoints(int currentHP);
                void setMaximumHitPoints(int maxHP);
        };
    }
}

#endif
