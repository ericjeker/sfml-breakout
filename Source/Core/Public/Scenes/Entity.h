// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>


class Entity
{
public:
    explicit Entity(const int id)
        : _id(id)
    {
    }
    ~Entity() = default;

    template <typename T>
    void AddComponent(T component)
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        components[std::type_index(typeid(T))] = std::make_unique<T>(std::move(component));
    }

    template <typename T>
    T* GetComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        const auto it = components.find(std::type_index(typeid(T)));
        return it != components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template <typename T>
    bool HasComponent() const
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        return components.contains(std::type_index(typeid(T)));
    }

    template <typename T>
    void RemoveComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        components.erase(std::type_index(typeid(T)));
    }

    int GetId() const
    {
        return _id;
    };

private:
    int _id = 0;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};


#endif
