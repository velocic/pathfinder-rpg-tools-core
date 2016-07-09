#ifndef CHARACTERCLASS_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <datatypes.h>
#include <observersubject.h>
#include <observer.h>
#include <preprocessordefines.h>

namespace RulesEngine
{
    namespace Character
    {
        //Should be moved into own class probably, with own observers
        struct CharacterClass
        {
            std::string className;
            unsigned int classLevel;
            unsigned int hitDieSize;
            unsigned int skillPointsPerLevel;

            //Multiplier * level to derive total BAB contribution from this class.
            //Options are 1, .75, and .5. Results rounded down.
            float baseAttackBonusProgression;
        };

        class PATHFINDER_RULES_ENGINE_EXPORT CharacterDescription :
            public ObserverSubject,
            public Observer
        {
            private:
                std::string characterName;
                std::string alignment;
                std::string player;
                std::unordered_map<std::string, CharacterClass> classInfo;
                std::string deity;
                std::string homeland;
                std::string race;
                std::string gender;
                std::string height;
                std::string hair;
                std::string eyes;
                unsigned int age = 0;
                unsigned int weight = 0;
                SizeCategories sizeCategory = SizeCategories::Medium; //Needs to be able subscribe for updates
                std::unordered_map<std::string, Observer*> observers;

                void notifyObservers(const std::string& fieldName) override;

            public:
                CharacterDescription() {}

                //Unregister from modules we're observing
                ~CharacterDescription() {}

                void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) override;
                void registerObserver(const std::string& observerName, Observer* observer) override;
                void unregisterObserver(const std::string& observerName) override;
                void addClass(const std::string& className);
                void addClass(const std::string& className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel, float baseAttackBonusProgression);
                void removeClass(const std::string& className);

                void setCharacterName(const std::string& name);
                void setAlignment(const std::string& alignment);
                void setPlayer(const std::string& playerName);
                void setClassLevel(const std::string& className, unsigned int classLevel);
                void setClassHitDieSize(const std::string& className, unsigned int hitDieSize);
                void setClassSkillPoints(const std::string& className, unsigned int skillPointsPerLevel);
                void setClassBaseAttackBonusProgression(const std::string& className, float progression);
                void setClass(const std::string& className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel, float baseAttackBonusProgression);
                void setDeity(const std::string& deityName);
                void setHomeland(const std::string& homeland);
                void setRace(const std::string& race);
                void setGender(const std::string& gender);
                void setHeight(const std::string& height);
                void setHair(const std::string& hair);
                void setEyes(const std::string& eyes);
                void setAge(unsigned int age);
                void setWeight(unsigned int weight);
                void setSizeCategory(SizeCategories size); //must notify observers of change

                std::string getCharacterName() const;
                std::string getAlignment() const;
                std::string getPlayer() const;
                CharacterClass getClass(const std::string& className) const;
                const std::unordered_map<std::string, CharacterClass>& getClasses() const;
                std::string getDeity() const;
                std::string getHomeland() const;
                std::string getRace() const;
                std::string getGender() const;
                std::string getHeight() const;
                std::string getHair() const;
                std::string getEyes() const;
                unsigned int getAge() const;
                unsigned int getWeight() const;
                SizeCategories getSizeCategory() const;
        };
    }
}

#define CHARACTERCLASS_H
#endif
