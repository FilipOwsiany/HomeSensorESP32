#include "IEventPublisherManager.h"

#include "CLogger.h"

IEventPublisherManager::IEventPublisherManager()
{
    CLogger::log(CLoggerModule::EventPublisherManager, CLoggerLevel::Debug, "IEventPublisherManager::IEventPublisherManager()");
}

IEventPublisherManager::~IEventPublisherManager()
{
    CLogger::log(CLoggerModule::EventPublisherManager, CLoggerLevel::Debug, "IEventPublisherManager::~IEventPublisherManager()");
}

void IEventPublisherManager::subscribe(IEventListener* listener)
{
    CLogger::log(CLoggerModule::EventPublisherManager, CLoggerLevel::Debug, "IEventPublisherManager::subscribe()");
    listeners.push_back(listener);
}

void IEventPublisherManager::unsubscribe(IEventListener* listener)
{
    CLogger::log(CLoggerModule::EventPublisherManager, CLoggerLevel::Debug, "IEventPublisherManager::unsubscribe()");
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end()
    );
}

void IEventPublisherManager::notify(SEvent& event)
{
    CLogger::log(CLoggerModule::EventPublisherManager, CLoggerLevel::Debug, "IEventPublisherManager::notify()");
    for (auto listener : listeners) {
        printf("IEventPublisherManager::notify() listener: %p\n", listener);
        listener->onEvent(event);
    }
}
