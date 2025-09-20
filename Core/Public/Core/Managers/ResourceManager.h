// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Utils/Logger.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

/**
 * @brief Simple resource manager that can load resources from a manifest file.
 */
class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    void LoadResourcesFromManifest(const std::string& manifestPath);
    void UnloadResource(const std::string& name);
    void CleanUp();

    template <typename T>
    std::shared_ptr<T> GetResource(const std::string& name)
    {
        const auto it = _resources.find(name);
        if (it == _resources.end())
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        // Get the resource from _resources
        auto* value = std::get_if<std::shared_ptr<T>>(&it->second);
        if (value == nullptr)
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        return *value;
    }

    template <typename T>
    bool SetResource(const std::string& name, std::shared_ptr<T> resource)
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
        std::shared_ptr<sf::Font>,
        std::shared_ptr<sf::Music>,
        std::shared_ptr<sf::Sound>,
        std::shared_ptr<sf::Shader>,
        std::shared_ptr<sf::Sprite>,
        std::shared_ptr<sf::Texture>>;

    std::unordered_map<std::string, ResourceVariant> _resources;
};
