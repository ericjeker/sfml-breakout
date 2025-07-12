// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Logger.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

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

	/** Templates are evaluated at compile time, so they need to be defined here */
	template <typename T>
	std::shared_ptr<T> GetResource(const std::string& name)
	{
		const auto it = _resources.find(name);
		if (it != _resources.end())
		{
			// Get the resource from _resources
			std::shared_ptr<T>* value = std::get_if<std::shared_ptr<T>>(&it->second);

			if (value == nullptr)
			{
				LOG_ERROR("Resource " + name + " not found.");
				return nullptr;
			}

			return *value;
		}

		LOG_ERROR("Resource " + name + " not found.");
		return nullptr;
	}

	/** Templates are evaluated at compile time, so they need to be defined here */
	template <typename T>
	void SetResource(const std::string& name, std::shared_ptr<T> resource)
	{
		_resources[name] = std::move(resource);
	}

private:
	using ResourceVariant =
		std::variant<std::shared_ptr<sf::RenderTexture>, std::shared_ptr<sf::Font>, std::shared_ptr<sf::Texture>,
					 std::shared_ptr<sf::Shader>, std::shared_ptr<sf::Sprite>, std::shared_ptr<sf::Text>,
					 std::shared_ptr<sf::CircleShape>>;

	std::unordered_map<std::string, ResourceVariant> _resources;
};

#endif
