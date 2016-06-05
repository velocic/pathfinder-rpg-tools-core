#ifndef OBSERVERSUBJECT_H
#define OBSERVERSUBJECT_H

#include <string>

class ObserverSubject
{
    public:
        virtual ~ObserverSubject() {}
        virtual void notifyObservers(std::string fieldName) = 0;
        virtual void registerObserver(std::string observerName, Observer* observer) = 0;
        virtual void unregisterObserver(std::string observerName, Observer* observer) = 0;
};

#endif
