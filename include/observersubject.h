#ifndef OBSERVERSUBJECT_H
#define OBSERVERSUBJECT_H

#include <string>
#include <observer.h>

class ObserverSubject
{
    public:
        virtual ~ObserverSubject() {}
        virtual void notifyObservers(const std::string& fieldName) = 0;
        virtual void registerObserver(const std::string& observerName, Observer* observer) = 0;
        virtual void unregisterObserver(const std::string& observerName) = 0;
};

#endif
