// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <functional>
#include <map>
#include <string>
#include <typeindex>
#include <vector>

// Examples:
struct PlayerDiedEvent
{
    int playerId;
    float x, y;
};
struct ScoreChangedEvent
{
    int newScore;
    int oldScore;
};

template <typename T>
using EventListener = std::function<void(const T& event, void* sender)>;

class EventManager
{
public:
    EventManager() = default;
    ~EventManager() = default;

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

        if (!_listeners.contains(typeIndex))
        {
            return;
        }

        for (auto& listener : _listeners[typeIndex])
        {
            listener(&event, sender);
        }
    }

private:
    using TypeErasedListener = std::function<void(const void* eventData, void* sender)>;
    std::map<std::type_index, std::vector<TypeErasedListener>> _listeners;
};


#endif
