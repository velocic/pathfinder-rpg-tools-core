#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <unordered_map>

#include <abilityscores.h>
#include <characterdescription.h>
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
                //Objects we're observing to calculate dependent fields
                AbilityScores& abilityScores;
                CharacterDescription& characterDescription;

                int currentHitPoints = 0;
                int maxHitPoints = 0;
                int negativeLevelPenalty = 0;
                int nonLethalDamage = 0;
                int temporaryHitPoints = 0;

                //If true, HP gains per level = 1/2 HD size + 1 per level + con modifier
                bool usePFSStyleFixedHPCalculation = false;

                //First level always = full HD size + con modifier
                std::vector<unsigned int> hpDieRollsByLevel;

                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;
            public:
                HitPoints(CharacterDescription& charDesc, AbilityScores& abilityScores);
                ~HitPoints();

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;

                //Generate Max HP from PFS rules (if user doesn't decide to roll themselves)
                // void autoGenerateMaximumHitPoints();
                void generateLevelUpHitPoints();

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

                void setUsePFSStyleFixedHPCalculation(bool shouldUse);
        };
    }
}

#endif
