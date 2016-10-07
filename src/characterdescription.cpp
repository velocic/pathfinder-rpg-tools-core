#include <characterdescription.h>

namespace RulesEngine
{
    namespace Character
    {
        void CharacterDescription::calculateNegativeLevels()
        {
            totalTemporaryNegativeLevels = 0;
            totalPermanentNegativeLevels = 0;
            totalNegativeLevels = 0;

            for (auto& negativeLevelDebuff : temporaryNegativeLevels) {
                if (negativeLevelDebuff.second.enabled == true) {
                    totalTemporaryNegativeLevels += negativeLevelDebuff.second.numNegativeLevels;
                }
            }

            for (auto& negativeLevelDebuff : permanentNegativeLevels) {
                if (negativeLevelDebuff.second.enabled == true) {
                    totalPermanentNegativeLevels += negativeLevelDebuff.second.numNegativeLevels;
                }
            }

            totalNegativeLevels = totalTemporaryNegativeLevels + totalPermanentNegativeLevels;

            notifyObservers("negativeLevels");
        }

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
            newClass.baseAttackBonusProgression = .5;

            classInfo.insert(
                std::make_pair(className, newClass)
            );

            notifyObservers("class");
        }

        void CharacterDescription::addClass(const std::string& className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel, float baseAttackBonusProgression)
        {
            CharacterClass newClass;
            newClass.className = className;
            newClass.classLevel = classLevel;
            newClass.hitDieSize = hitDieSize;
            newClass.skillPointsPerLevel = skillPointsPerLevel;
            newClass.baseAttackBonusProgression = baseAttackBonusProgression;

            classInfo.insert(
                std::make_pair(className, std::move(newClass))
            );

            notifyObservers("class");
        }

        void CharacterDescription::addTemporaryNegativeLevels(const std::string& sourceName, unsigned int amountToAdd)
        {
            auto mapIterator = temporaryNegativeLevels.find(sourceName);

            if (mapIterator == temporaryNegativeLevels.end()) {
                return;
            }

            auto& negativeLevelDebuff = mapIterator->second;

            negativeLevelDebuff.numNegativeLevels += amountToAdd;

            calculateNegativeLevels();
        }

        void CharacterDescription::addPermanentNegativeLevels(const std::string& sourceName, unsigned int amountToAdd)
        {
            auto mapIterator = permanentNegativeLevels.find(sourceName);

            if (mapIterator == permanentNegativeLevels.end()) {
                return;
            }

            auto& negativeLevelDebuff = mapIterator->second;

            negativeLevelDebuff.numNegativeLevels += amountToAdd;

            calculateNegativeLevels();
        }

        void CharacterDescription::addTemporaryNegativeLevelDebuff(const std::string& sourceName, unsigned int numNegativeLevels, const std::string& description)
        {
            TemporaryNegativeLevelDebuff tempDebuff;
            tempDebuff.description = description;
            tempDebuff.sourceName = sourceName;
            tempDebuff.numNegativeLevels = numNegativeLevels;
            tempDebuff.enabled = true;

            temporaryNegativeLevels[sourceName] = std::move(tempDebuff);

            calculateNegativeLevels();
        }

        void CharacterDescription::addPermanentNegativeLevelDebuff(const std::string& sourceName, unsigned int numNegativeLevels, const std::string& description)
        {
            PermanentNegativeLevelDebuff permDebuff;
            permDebuff.description = description;
            permDebuff.sourceName = sourceName;
            permDebuff.numNegativeLevels = numNegativeLevels;
            permDebuff.enabled = true;

            permanentNegativeLevels[sourceName] = std::move(permDebuff);

            calculateNegativeLevels();
        }

        void CharacterDescription::removeClass(const std::string& className)
        {
            classInfo.erase(className);

            notifyObservers("class");
        }

        void CharacterDescription::removeTemporaryNegativeLevels(unsigned int amountToRemove)
        {
            int removalCounter = static_cast<int>(amountToRemove);

            for (auto& temporaryNegLevelDebuffPair : temporaryNegativeLevels) {
                if (removalCounter <= 0) {
                    break;
                }

                auto& negLevelDebuff = temporaryNegLevelDebuffPair.second;

                if (removalCounter >= static_cast<int>(negLevelDebuff.numNegativeLevels)) {
                    removalCounter -= negLevelDebuff.numNegativeLevels;
                    negLevelDebuff.numNegativeLevels = 0;
                    negLevelDebuff.enabled = false;
                    continue;
                }

                negLevelDebuff.numNegativeLevels -= removalCounter;
                removalCounter = 0;
                break;
            }

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        void CharacterDescription::removePermanentNegativeLevels(unsigned int amountToRemove)
        {
            int removalCounter = static_cast<int>(amountToRemove);

            for (auto& permanentNegLevelDebuffPair : permanentNegativeLevels) {
                if (removalCounter <= 0) {
                    break;
                }

                auto& negLevelDebuff = permanentNegLevelDebuffPair.second;

                if (removalCounter >= static_cast<int>(negLevelDebuff.numNegativeLevels)) {
                    removalCounter -= negLevelDebuff.numNegativeLevels;
                    negLevelDebuff.numNegativeLevels = 0;
                    negLevelDebuff.enabled = false;
                    continue;
                }

                negLevelDebuff.numNegativeLevels -= removalCounter;
                removalCounter = 0;
                break;
            }

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        void CharacterDescription::removeTemporaryNegativeLevelDebuff(const std::string& sourceName)
        {
            temporaryNegativeLevels.erase(sourceName);

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        void CharacterDescription::removePermanentNegativeLevelDebuff(const std::string& sourceName)
        {
            permanentNegativeLevels.erase(sourceName);

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        void CharacterDescription::setCharacterStatus(CharacterStatus status)
        {
            characterStatus = status;
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
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return;
            }

            mapIterator->second.classLevel = classLevel;

            notifyObservers("class");
        }

        void CharacterDescription::setClassHitDieSize(const std::string& className, unsigned int hitDieSize)
        {
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return;
            }

            mapIterator->second.hitDieSize = hitDieSize;

            notifyObservers("class");
        }

        void CharacterDescription::setClassSkillPoints(const std::string& className, unsigned int skillPointsPerLevel)
        {
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return;
            }

            mapIterator->second.skillPointsPerLevel = skillPointsPerLevel;

            notifyObservers("class");
        }

        void CharacterDescription::setClassBaseAttackBonusProgression(const std::string& className, float progression)
        {
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return;
            }

            mapIterator->second.baseAttackBonusProgression = progression;

            notifyObservers("class");
        }

        void CharacterDescription::setClass(const std::string& className, unsigned int classLevel, unsigned int hitDieSize, unsigned int skillPointsPerLevel, float baseAttackBonusProgression)
        {
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return;
            }

            CharacterClass& classToUpdate = mapIterator->second;
            classToUpdate.classLevel = classLevel;
            classToUpdate.hitDieSize = hitDieSize;
            classToUpdate.skillPointsPerLevel = skillPointsPerLevel;
            classToUpdate.baseAttackBonusProgression = baseAttackBonusProgression;

            notifyObservers("class");
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

        void CharacterDescription::toggleTemporaryNegativeLevelDebuff(const std::string& sourceName)
        {
            auto mapIterator = temporaryNegativeLevels.find(sourceName);

            if (mapIterator == temporaryNegativeLevels.end()) {
                return;
            }

            auto& enabled = mapIterator->second.enabled;

            if (enabled == true) {
                enabled = false;
            } else {
                enabled = true;
            }

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        void CharacterDescription::togglePermanentNegativeLevelDebuff(const std::string& sourceName)
        {
            auto mapIterator = permanentNegativeLevels.find(sourceName);

            if (mapIterator == permanentNegativeLevels.end()) {
                return;
            }

            auto& enabled = mapIterator->second.enabled;

            if (enabled == true) {
                enabled = false;
            } else {
                enabled = true;
            }

            //No need to call notify here, calculateNegativeLevels calls it for us
            calculateNegativeLevels();
        }

        CharacterStatus CharacterDescription::getCharacterStatus() const
        {
            return characterStatus;
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
            auto mapIterator = classInfo.find(className);

            if (mapIterator == classInfo.end()) {
                return CharacterClass();
            }

            return mapIterator->second;
        }

        const std::unordered_map<std::string, CharacterClass>& CharacterDescription::getClasses() const
        {
            return classInfo;
        }

        TemporaryNegativeLevelDebuff CharacterDescription::getTemporaryNegativeLevelDebuff(const std::string& sourceName) const
        {
            auto mapIterator = temporaryNegativeLevels.find(sourceName);

            if (mapIterator == temporaryNegativeLevels.end()) {
                return TemporaryNegativeLevelDebuff();
            }

            return mapIterator->second;
        }

        PermanentNegativeLevelDebuff CharacterDescription::getPermanentNegativeLevelDebuff(const std::string& sourceName) const
        {
            auto mapIterator = permanentNegativeLevels.find(sourceName);

            if (mapIterator == permanentNegativeLevels.end()) {
                return PermanentNegativeLevelDebuff();
            }

            return mapIterator->second;
        }

        const std::unordered_map<std::string, TemporaryNegativeLevelDebuff>& CharacterDescription::getTemporaryNegativeLevelDebuffs() const
        {
            return temporaryNegativeLevels;
        }

        const std::unordered_map<std::string, PermanentNegativeLevelDebuff>& CharacterDescription::getPermanentNegativeLevelDebuffs() const
        {
            return permanentNegativeLevels;
        }

        unsigned int CharacterDescription::getTotalNegativeLevels()
        {
            return totalNegativeLevels;
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
