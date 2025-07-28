// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Logger.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <variant>

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    void LoadResourcesFromManifest(const std::string& manifestPath);
    void UnloadResource(const std::string& name);
    void CleanUp();

    template <typename T>
    T* GetResource(const std::string& name)
    {
        const auto it = _resources.find(name);
        if (it == _resources.end())
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        // Get the resource from _resources
        auto* value = std::get_if<std::unique_ptr<T>>(&it->second);

        if (value == nullptr)
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        return value->get();
    }

    template <typename T>
    bool SetResource(const std::string& name, std::unique_ptr<T> resource)
    {
        if (!resource)
        {
            LOG_ERROR("Attempted to set null resource: " + name);
            return false;
        }

        _resources[name] = std::move(resource);
        return true;
    }

private:
    using ResourceVariant = std::variant<
        std::unique_ptr<sf::Font>,
        std::unique_ptr<sf::Music>,
        std::unique_ptr<sf::Sound>,
        std::unique_ptr<sf::Shader>,
        std::unique_ptr<sf::Sprite>,
        std::unique_ptr<sf::Texture>>;

    std::unordered_map<std::string, ResourceVariant> _resources;
};

#endif
