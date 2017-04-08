#include <initiative.h>

namespace RulesEngine
{
    namespace Character
    {
        Initiative::Initiative(AbilityScores& abilityScores)
        :
            abilityScores(abilityScores)
        {
            abilityScores.registerObserver("initiative", this);
            calculateTotalInitiative();
        }

        Initiative::~Initiative()
        {
            abilityScores.unregisterObserver("initiative");
        }

        void Initiative::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void Initiative::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
            if (fieldName == "dexterity") {
                calculateTotalInitiative();
            }
        }

        void Initiative::registerObserver(const std::string& observerName, Observer* observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void Initiative::unregisterObserver(const std::string& observerName)
        {
            observers.erase(observerName);
        }

        void Initiative::addInitiativeModifier(const std::string& sourceName, const std::string& description, int modifierValue, bool enabled)
        {
            auto modifier = InitiativeModifier{description, sourceName, modifierValue, enabled};
            auto tempPair = std::make_pair(description, modifier);

            miscInitiativeModifiers.insert(tempPair);

            calculateTotalInitiative();
        }

        void Initiative::calculateTotalInitiative()
        {
            int modifierCount = 0;
            for (const auto& modifier : miscInitiativeModifiers) {
                if (modifier.second.enabled) {
                    modifierCount += modifier.second.modifierValue;
                }
            }
            modifierCount += abilityScores.getBaseModifierWithPermanentAdjustments(AbilityScoreTypes::DEX);

            totalInitiative = modifierCount;

            notifyObservers("initiative");
        }

        int Initiative::getInitiative() const
        {
            return totalInitiative;
        }
    }
}
