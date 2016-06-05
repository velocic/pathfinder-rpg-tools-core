#ifndef OBSERVERSUBJECT_H
#define OBSERVERSUBJECT_H

#include <string>
#include <observer.h>

namespace RulesEngine
{
    class ObserverSubject
    {
        private:
            virtual void notifyObservers(const std::string& fieldName) = 0;

        public:
            virtual ~ObserverSubject() {}
            virtual void registerObserver(const std::string& observerName, Observer* observer) = 0;
            virtual void unregisterObserver(const std::string& observerName) = 0;
    };
}

#endif
