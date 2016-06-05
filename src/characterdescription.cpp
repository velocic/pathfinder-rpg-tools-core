#include <characterdescription.h>

namespace RulesEngine
{
    namespace Character
    {
        void CharacterDescription::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void CharacterDescription::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //Update dependent fields when notified from other systems once implemented
        }

        void CharacterDescription::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(
                std::make_pair(observerName, observer)
            );
        }

        void CharacterDescription::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void CharacterDescription::addClass(const std::string& className)
        {
            CharacterClass newClass;
            newClass.className = className;
            newClass.classLevel = 1;
            newClass.hitDieSize = 4;
            newClass.skillPointsPerLevel = 2;

            classInfo.insert(
                std::make_pair(className, newClass)
            );
        }

        void CharacterDescription::setCharacterName(const std::string& name)
        {
            characterName = name;
        }

        void CharacterDescription::setAlignment(const std::string& alignment)
        {
            this->alignment = alignment;
        }

        void CharacterDescription::setPlayer(const std::string& playerName)
        {
            player = playerName;
        }

        void CharacterDescription::setClassLevel(const std::string& className, unsigned int classLevel)
        {
            classInfo.find(className)->second.classLevel = classLevel;
        }

        void CharacterDescription::setClassHitDieSize(const std::string& className, unsigned int hitDieSize)
        {
            classInfo.find(className)->second.hitDieSize = hitDieSize;
        }

        void CharacterDescription::setClassSkillPoints(const std::string& className, unsigned int skillPointsPerLevel)
        {
            classInfo.find(className)->second.skillPointsPerLevel = skillPointsPerLevel;
        }

        void CharacterDescription::setClass(const std::string& className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel)
        {
            CharacterClass& classToUpdate = classInfo.find(className)->second;
            classToUpdate.classLevel = classLevel;
            classToUpdate.hitDieSize = hitDieSize;
            classToUpdate.skillPointsPerLevel = skillPointsPerLevel;
        }

        void CharacterDescription::setDeity(const std::string& deityName)
        {
            deity = deityName;
        }

        void CharacterDescription::setHomeland(const std::string& homeland)
        {
            this->homeland = homeland;
        }

        void CharacterDescription::setRace(const std::string& race)
        {
            this->race = race;
        }

        void CharacterDescription::setGender(const std::string& gender)
        {
            this->gender = gender;
        }

        void CharacterDescription::setHeight(const std::string& height)
        {
            this->height = height;
        }

        void CharacterDescription::setHair(const std::string& hair)
        {
            this->hair = hair;
        }

        void CharacterDescription::setEyes(const std::string& eyes)
        {
            this->eyes = eyes;
        }

        void CharacterDescription::setAge(unsigned int age)
        {
            this->age = age;
        }

        void CharacterDescription::setWeight(unsigned int weight)
        {
            this->weight = weight;
        }

        void CharacterDescription::setSizeCategory(SizeCategories size)
        {
            sizeCategory = size;
            notifyObservers("sizeCategory");
        }

        std::string CharacterDescription::getCharacterName() const
        {
            return characterName;
        }

        std::string CharacterDescription::getAlignment() const
        {
            return alignment;
        }

        std::string CharacterDescription::getPlayer() const
        {
            return player;
        }

        CharacterClass CharacterDescription::getClass(const std::string& className) const
        {
            return classInfo.find(className)->second;
        }

        std::string CharacterDescription::getDeity() const
        {
            return deity;
        }

        std::string CharacterDescription::getHomeland() const
        {
            return homeland;
        }

        std::string CharacterDescription::getRace() const
        {
            return race;
        }

        std::string CharacterDescription::getGender() const
        {
            return gender;
        }

        std::string CharacterDescription::getHeight() const
        {
            return height;
        }

        std::string CharacterDescription::getHair() const
        {
            return hair;
        }

        std::string CharacterDescription::getEyes() const
        {
            return eyes;
        }

        unsigned int CharacterDescription::getAge() const
        {
            return age;
        }

        unsigned int CharacterDescription::getWeight() const
        {
            return weight;
        }

        SizeCategories CharacterDescription::getSizeCategory() const
        {
            return sizeCategory;
        }
    }
}
