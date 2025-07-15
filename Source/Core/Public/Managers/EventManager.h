// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include <functional>
#include <map>
#include <string>
#include <vector>



class EventManager {
public:
    void Subscribe(const std::string& eventType, std::function<void()> listener) {
        _listeners[eventType].push_back(listener);
    }

    void Trigger(const std::string& eventType, void* sender) {
        if (_listeners.find(eventType) != _listeners.end()) {
            for (auto& listener : _listeners[eventType]) {
                listener(); // Call the subscribed function
            }
        }
    }

private:
    // Using std::function to store any callable target
    std::map<std::string, std::vector<std::function<void()>>> _listeners;

};


#endif
