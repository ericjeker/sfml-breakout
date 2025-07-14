// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GAMESERVICES_H
#define GAMESERVICES_H

#include "Logger.h"

#include <any>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

class GameService
{
public:
    GameService();
    ~GameService() = default;

    // TODO: use an interface and implementation instead
    template <typename T>
    void Register(std::unique_ptr<T> service)
    {
        const std::type_index typeIndex(typeid(T));

        if (_services.contains(typeIndex))
        {
            LOG_ERROR("(GameService): Service " + std::string(typeid(T).name()) + " already exists.");
            return;
        }

        // Wrap the service so its type disappears
        _services[typeIndex] = std::make_unique<ServiceWrapper<T>>(std::move(service));
    }

    template <typename T>
    T* Get()
    {
        const std::type_index typeIndex(typeid(T));

        if (const auto it = _services.find(typeIndex); it != _services.end())
        {
            auto* wrapper = static_cast<ServiceWrapper<T>*>(it->second.get());
            return wrapper->service.get();
        }

        return nullptr;
    }

    template <typename T>
    bool IsRegistered() const
    {
        return _services.contains(std::type_index(typeid(T)));
    }

private:
    // We use type erasure here to avoid having to use std::any
    struct ServiceBase
    {
        virtual ~ServiceBase() = default;
    };

    template <typename T>
    struct ServiceWrapper final : ServiceBase
    {
        std::unique_ptr<T> service;
        explicit ServiceWrapper(std::unique_ptr<T> s)
            : service(std::move(s))
        {
        }
    };

    std::unordered_map<std::type_index, std::unique_ptr<ServiceBase>> _services;
};


#endif
