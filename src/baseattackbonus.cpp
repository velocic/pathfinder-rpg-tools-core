#include <baseattackbonus.h>

namespace RulesEngine
{
    namespace Character
    {
        void BaseAttackBonus::calculateBaseAttackBonus(const CharacterDescription& characterDescription)
        {
            auto characterClassMap = characterDescription.getClasses();

            unsigned int totalBAB = 0;
            baseAttackBonusByClass.clear();

            for (auto characterClassPair : characterClassMap) {
                auto classBABContribution = characterClassPair.second.baseAttackBonusProgression * characterClassPair.second.classLevel;

                totalBAB += classBABContribution;

                baseAttackBonusByClass.emplace_back(
                    characterClassPair.second.className,
                    classBABContribution
                );
            }

            totalBaseAttackBonus = totalBAB;

            notifyObservers("baseAttackBonus");
        }

        void BaseAttackBonus::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        unsigned int BaseAttackBonus::getTotalBaseAttackBonus() const
        {
            return totalBaseAttackBonus;
        }

        const std::vector<std::pair<std::string, unsigned int>>& BaseAttackBonus::getBaseAttackBonusByClass() const
        {
            return baseAttackBonusByClass;
        }

        std::vector<unsigned int> BaseAttackBonus::getIterativeAttackBonuses() const
        {
            int baseAttackBonusCounter = static_cast<int>(totalBaseAttackBonus);
            std::vector<unsigned int> iterativeBonuses;

            for (; baseAttackBonusCounter > 0; baseAttackBonusCounter -= 5) {
                iterativeBonuses.push_back(static_cast<unsigned int>(baseAttackBonusCounter));
            }

            return iterativeBonuses;
        }

        void BaseAttackBonus::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            //Provided by CharacterDescription
            if (fieldName == "class") {
                calculateBaseAttackBonus(*static_cast<const CharacterDescription*>(subject));
            }
        }

        void BaseAttackBonus::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void BaseAttackBonus::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }
    }
}
