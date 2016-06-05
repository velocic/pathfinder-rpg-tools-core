#include <characterdescription.h>

void CharacterDescription::notifyObservers(std::string fieldName)
{
    for (auto observer : observers) {
        observer->second->receiveNotification(this, fieldName);
    }
}

void CharacterDescription::registerObserver(std::string observerName, Observer* observer)
{
    observers.insert(
        std::make_pair<std::string, Observer*>(observerName, observer)
    );
}

void CharacterDescription::unregisterObserver(std::string observerName)
{
    observers.erase(observerName);
}

void addClass(std::string className)
{
    CharacterClass newClass;
    newClass.className = className;
    newClass.classLevel = 1;
    newClass.hitDieSize = 4;
    newClass.skillPointsPerLevel = 2;

    classInfo.insert(
        std::make_pair<std::string, CharacterClass>(newClass)
    );
}

void CharacterDescription::setCharacterName(std::string name)
{
    characterName = name;
}

void CharacterDescription::setAlignment(std::string alignment)
{
    this->alignment = alignment;
}

void CharacterDescription::setPlayer(std::string playerName)
{
    player = playerName;
}

void CharacterDescription::setClassLevel(std::string className, unsigned int classLevel)
{
    classInfo.find(className)->second.classLevel = classLevel;
}

void CharacterDescription::setClassHitDieSize(std::string className, unsigned int hitDieSize)
{
    classInfo.find(className)->second.hitDieSize = hitDieSize;
}

void CharacterDescription::setClassSkillPoints(std::string className, unsigned int skillPointsPerLevel)
{
    classInfo.find(className)->second.skillPointsPerLevel = skillPointsPerLevel;
}

void CharacterDescription::setClass(std::string className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel)
{
    CharacterClass& classToUpdate = classInfo.find(className)->second;
    classToUpdate.classLevel = classLevel;
    classToUpdate.hitDieSize = hitDieSize;
    classToUpdate.skillPointsPerLevel = skillPointsPerLevel;
}

void CharacterDescription::setDeity(std::string deityName)
{
    deity = deityName;
}

void CharacterDescription::setHomeland(std::string homeland)
{
    this->homeland = homeland;
}

void CharacterDescription::setRace(std::string race)
{
    this->race = race;
}

void CharacterDescription::setGender(std::string gender)
{
    this->gender = gender;
}

void CharacterDescription::setHeight(std::string height)
{
    this->height = height;
}

void CharacterDescription::setHair(std::string hair)
{
    this->hair = hair;
}

void CharacterDescription::setEyes(std::string eyes)
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
}

std::string CharacterDescription::getCharacterName()
{
    return characterName;
}

std::string CharacterDescription::getAlignment()
{
    return alignment;
}

std::string CharacterDescription::getPlayer()
{
    return player;
}

CharacterClass CharacterDescription::getClass(std::string className)
{
    return classInfo.find(className)->second;
}

std::string CharacterDescription::getDeity()
{
    return deity;
}

std::string CharacterDescription::getHomeland()
{
    return homeland;
}

std::string CharacterDescription::getRace()
{
    return race;
}

std::string CharacterDescription::getGender()
{
    return gender;
}

std::string CharacterDescription::getHeight()
{
    return height;
}

std::string CharacterDescription::getHair()
{
    return hair;
}

std::string CharacterDescription::getEyes()
{
    return eyes;
}

unsigned int CharacterDescription::getAge()
{
    return age;
}

unsigned int CharacterDescription::getWeight()
{
    return weight;
}

SizeCategories CharacterDescription::getSizeCategory()
{
    return sizeCategory;
}
