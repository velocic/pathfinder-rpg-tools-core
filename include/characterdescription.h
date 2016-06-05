#ifndef CHARACTERCLASS_H

#include <string>
#include <vector>
#include <unordered_map>

#include <datatypes.h>
#include <observersubject.h>
#include <observer.h>

//Should be moved into own class probably, with own observers
struct CharacterClass
{
    std::string className;
    unsigned int classLevel;
    unsigned int hitDieSize;
    unsigned int skillPointsPerLevel;
};

class CharacterDescription :
    ObserverSubject,
    Observer
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

        void notifyObservers();

    public:
        CharacterDescription() {};

        void registerObserver(std::string observerName, Observer* observer);
        void unregisterObserver(std::string observerName, Observer* observer);
        void addClass(std::string className);
        void setCharacterName(std::string name);
        void setAlignment(std::string alignment);
        void setPlayer(std::string playerName);
        void setClassLevel(std::string className, unsigned int classLevel);
        void setClassHitDieSize(std::string className, unsigned int hitDieSize);
        void setClassSkillPoints(std::string className, unsigned int skillPointsPerLevel);
        void setClass(std::string className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel);
        void setDeity(std::string deityName);
        void setHomeland(std::string homeland);
        void setRace(std::string race);
        void setGender(std::string gender);
        void setHeight(std::string height);
        void setHair(std::string hair);
        void setEyes(std::string eyes);
        void setAge(unsigned int age);
        void setWeight(unsigned int weight);
        void setSizeCategory(SizeCategories size); //must notify observers of change

        std::string getCharacterName();
        std::string getAlignment();
        std::string getPlayer();
        CharacterClass getClass();

        std::string getDeity();
        std::string getHomeland();
        std::string getRace();
        std::string getGender();
        std::string getHeight();
        std::string getHair();
        std::string getEyes();
        unsigned int getAge();
        unsigned int getWeight();
        SizeCategories getSizeCategory();
};


#define CHARACTERCLASS_H
#endif
