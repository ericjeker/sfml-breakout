// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/EventManager.h"

void EventManager::ProcessDeferredEvents()
{
    // Process all deferred events
    for (auto& deferredEvent : _deferredEvents)
    {
        LOG_DEBUG(
            "(EventManager::ProcessDeferredEvents: Processing deferred event: " +
            std::string(deferredEvent.eventType.name())
        );
        deferredEvent.eventExecutor();
    }

    // Clear the deferred events after processing
    _deferredEvents.clear();
}
