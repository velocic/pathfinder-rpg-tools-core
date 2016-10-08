#ifndef HITPOINTS_H
#define HITPOINTS_H

#include <random>
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
                int nonLethalDamage = 0;
                int temporaryHitPoints = 0;

                //If true, HP gains per level = 1/2 HD size + 1 per level + con modifier
                bool usePFSStyleFixedHPCalculation = false;

                //Kludgy flag for making sure the very first character level always
                //is assigned the max possible value
                bool firstHitDieRollCalculated = false;

                //Map of class names -> HP die rolls, in order by level
                std::unordered_map<std::string, std::vector<unsigned int>> hpDieRollsByClass;

                std::unordered_map<std::string, Observer*> observers;

                void resetMaxHPIfCharacterClassRemoved(const std::unordered_map<std::string, CharacterClass>& characterClasses);
                void setMaxHPFromDieRolls(const std::unordered_map<std::string, std::vector<unsigned int>>& hpDieRollsByClass);

                void generateHitPointsPFSRules();
                void generateHitPointsCoreRules();

                void notifyObservers(const std::string& fieldName) override;
            public:
                HitPoints(CharacterDescription& charDesc, AbilityScores& abilityScores);
                ~HitPoints();

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;

                //Generate Max HP from PFS rules (if user doesn't decide to roll themselves)
                // void autoGenerateMaximumHitPoints();
                void generateHitPoints();

                void addTemporaryHitPoints(int tempHP);

                void dealNonLethalDamage(int damageAmount);
                void dealDamage(int damageAmount);
                void healHitPoints(int healAmount);

                int getCurrentHitPoints() const;
                const std::unordered_map<std::string, std::vector<unsigned int>>& getHpDieRollsByClass() const;
                int getNonLethalDamage() const;
                int getMaxHitPoints() const;
                int getTemporaryHitPoints() const;

                void recalculateTotalHitPoints();
                void reset();
                void resetNonLethalDamage();
                void resetTemporaryHitPoints();

                void setCurrentHitPoints(int currentHP);
                void setMaximumHitPoints(int maxHP);

                void setUsePFSStyleFixedHPCalculation(bool shouldUse);
        };
    }
}

#endif
