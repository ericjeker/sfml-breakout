// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Logger.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
    std::shared_ptr<T> GetResource(const std::string& name)
    {
        const auto it = _resources.find(name);
        if (it == _resources.end())
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        // Get the resource from _resources
        std::shared_ptr<T>* value = std::get_if<std::shared_ptr<T>>(&it->second);

        if (value == nullptr)
        {
            LOG_ERROR("Resource " + name + " not found.");
            return nullptr;
        }

        return *value;
    }

    template <typename T>
    void SetResource(const std::string& name, std::shared_ptr<T> resource)
    {
        _resources[name] = std::move(resource);
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

#endif
