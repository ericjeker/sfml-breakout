// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Utils/Logger.h"

#include <any>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

/**
 * @brief Service locator pattern implementation for game services management.
 *
 * GameService provides a centralized registry for managing and accessing game services.
 * It supports both unique pointer and reference registration of services.
 * Services are stored using type erasure to avoid dependencies on std::any.
 *
 * This namespace replicates a Singleton:
 *
 * GameService::Register<IManager>(std::move(mgrImpl));
 * GameService::Get<IManager>();
 */
namespace GameService
{

/**
 * @brief Base class for service wrappers using type erasure pattern.
 *
 * We use type erasure here to avoid using std::any
 */
struct ServiceBase
{
    virtual ~ServiceBase() = default;
};

/**
 * @brief Wrapper for services
 * @tparam T The service type to wrap.
 */
template <typename T>
struct ServiceWrapper final : ServiceBase
{
    std::unique_ptr<T> service;
    T* servicePtr;

    explicit ServiceWrapper(std::unique_ptr<T> s)
        : service(std::move(s))
        , servicePtr(service.get())
    {
    }

    explicit ServiceWrapper(T& s)
        : service(nullptr)
        , servicePtr(&s)
    {
    }

    T& Get()
    {
        return *servicePtr;
    }
};

// Type definition
using ServiceMap = std::unordered_map<std::type_index, std::unique_ptr<ServiceBase>>;
inline ServiceMap services;

inline void Initialize()
{
    LOG_DEBUG("GameService::Initialize");
}

inline void Shutdown()
{
    LOG_DEBUG("GameService::Shutdown");
    services.clear();
}

inline ServiceMap& GetServices()
{
    return services;
}

/**
 * @brief Registers a service using a unique pointer.
 * @tparam T The type of service to register.
 * @param service The unique pointer to the service instance.
 */
template <typename T>
void Register(std::unique_ptr<T> service)
{
    const std::type_index typeIndex(typeid(T));

    if (GetServices().contains(typeIndex))
    {
        LOG_ERROR("GameService: Service " + std::string(typeid(T).name()) + " already exists.");
        return;
    }

    // Wrap the service so its type disappears
    GetServices()[typeIndex] = std::make_unique<ServiceWrapper<T>>(std::move(service));
}

/**
 * @brief Registers a service using a reference.
 * @tparam T The type of service to register.
 * @param service The reference to the service instance.
 */
template <typename T>
void Register(T& service)
{
    const std::type_index typeIndex(typeid(T));

    if (GetServices().contains(typeIndex))
    {
        LOG_ERROR("GameService: Service " + std::string(typeid(T).name()) + " already exists.");
        return;
    }

    GetServices()[typeIndex] = std::make_unique<ServiceWrapper<T>>(service);
}

/**
 * @brief Retrieves a registered service.
 * @tparam T The type of service to retrieve.
 * @return Reference to the requested service.
 * @throws std::runtime_error if the service is not found.
 */
template <typename T>
T& Get()
{
    const std::type_index typeIndex(typeid(T));

    if (const auto it = services.find(typeIndex); it != services.end())
    {
        auto* wrapper = static_cast<ServiceWrapper<T>*>(it->second.get());
        return wrapper->Get();
    }

    throw std::runtime_error("Game Service not found: " + std::string(typeid(T).name()));
}


} // namespace GameService


