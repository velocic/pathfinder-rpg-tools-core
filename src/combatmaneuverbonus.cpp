#include <combatmaneuverbonus.h>

namespace RulesEngine
{
    namespace Character
    {
        CombatManeuverBonus::CombatManeuverBonus(CharacterDescription& description, AbilityScores& abilityScores)
        :
            characterDescription(description),
            abilityScores(abilityScores)
        {
            characterDescription.registerObserver("combatManeuverBonus", this);
            abilityScores.registerObserver("combatManeuverBonus", this);

            calculateTotalCombatManeuverBonus();
        }

        CombatManeuverBonus::~CombatManeuverBonus()
        {
            characterDescription.unregisterObserver("combatManeuverBonus");
            abilityScores.unregisterObserver("combatManeuverBonus");
        }

        void CombatManeuverBonus::notifyObservers(const std::string& fieldName)
        {
            for (auto observer : observers) {
                observer.second->receiveNotification(this, fieldName);
            }
        }

        void CombatManeuverBonus::receiveNotification(const ObserverSubject* subject, const std::string& fieldName)
        {
        }

        void CombatManeuverBonus::registerObserver(const std::string &observerName, Observer *observer)
        {
            observers.insert(std::make_pair(observerName, observer));
        }

        void CombatManeuverBonus::unregisterObserver(const std::string &observerName)
        {
            observers.erase(observerName);
        }

        void CombatManeuverBonus::addCMBModifier(CombatManeuverModifierType type, const std::string& sourceName, const std::string& description, int modifierValue, bool enabled)
        {
            CMBModifiers[sourceName] = CombatManeuverModifier{type, description, sourceName, modifierValue, enabled};

            calculateTotalCombatManeuverBonus();
        }

        void CombatManeuverBonus::calculateTotalCombatManeuverBonus()
        {
            notifyObservers("combatManeuverBonus");
        }

        int CombatManeuverBonus::getCombatManeuverBonus() const
        {
            return totalCombatManeuverBonus;
        }

    }
}
