#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

#include <preprocessordefines.h>

namespace RulesEngine
{
    //Forward declare to avoid circular dependency
    class ObserverSubject;

    class PATHFINDER_RULES_ENGINE_EXPORT Observer
    {
        public:
            virtual ~Observer() {}

            virtual void receiveNotification(const ObserverSubject* subject, const std::string& fieldName) = 0;
    };
}

#endif
