#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

class Observer
{
    public:
        virtual ~Observer() {}

        virtual void receiveNotification(ObserverSubject* subject, std::string fieldName);
};

#endif
