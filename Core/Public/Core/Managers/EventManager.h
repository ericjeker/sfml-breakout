// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Utils/Logger.h"

#include <functional>
#include <map>
#include <string>
#include <typeindex>
#include <vector>

template <typename T>
using EventListener = std::function<void(const T& event, void* sender)>;

/**
 * @brief PubSub system, where event can be processes immediately or deferred.
 */
class EventManager
{
public:
    EventManager() = default;
    ~EventManager() = default;

    using TypeErasedListener = std::function<void(const void* eventData, void* sender)>;
    using TypeErasedEvent = std::function<void(void* sender)>;

    // Structure to hold deferred event information
    struct DeferredEvent
    {
        std::type_index eventType;
        std::function<void()> eventExecutor;

        DeferredEvent(std::type_index type, std::function<void()> executor)
            : eventType(type)
            , eventExecutor(std::move(executor))
        {
        }
    };

    template <typename T>
    void Subscribe(EventListener<T> listener)
    {
        const std::type_index typeIndex(typeid(T));
        auto wrapper = [listener](const void* eventData, void* sender)
        {
            const T* typedEvent = static_cast<const T*>(eventData);
            listener(*typedEvent, sender);
        };
        _listeners[typeIndex].push_back(wrapper);
    }

    template <typename T>
    void Unsubscribe(EventListener<T> listener)
    {
        const std::type_index typeIndex(typeid(T));

        if (!_listeners.contains(typeIndex))
        {
            return;
        }

        _listeners[typeIndex]
            .erase(std::remove(std::begin(_listeners[typeIndex]), std::end(_listeners[typeIndex]), listener), std::end(_listeners[typeIndex]));
    }

    template <typename T>
    void Emit(const T& event, void* sender)
    {
        const std::type_index typeIndex(typeid(T));

        LOG_DEBUG("EventManager::Emit: Emitting event: " + std::string(typeid(T).name()));

        if (!_listeners.contains(typeIndex))
        {
            return;
        }

        for (auto& listener : _listeners[typeIndex])
        {
            listener(&event, sender);
        }
    }

    template <typename T>
    void EmitDeferred(const T& event, void* sender)
    {
        const std::type_index typeIndex(typeid(T));

        LOG_DEBUG("EventManager::EmitDeferred: Queueing event: " + std::string(typeid(T).name()));

        // Create a copy of the event and capture it in the lambda
        T eventCopy = event;
        auto eventExecutor = [this, eventCopy, sender]() mutable { this->Emit(eventCopy, sender); };

        _deferredEvents.emplace_back(typeIndex, std::move(eventExecutor));
    }

    void ProcessDeferredEvents();
private:
    std::map<std::type_index, std::vector<TypeErasedListener>> _listeners;
    std::vector<DeferredEvent> _deferredEvents;
};


