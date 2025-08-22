// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/ResourceManager.h"

#include "Core/Utils/Logger.h"
#include "Core/Managers/FileManager.h"

#include <nlohmann/json.hpp>

void ResourceManager::LoadResourcesFromManifest(const std::string& manifestPath)
{
    try
    {
        const json manifest = FileManager::LoadJSON(manifestPath);

        if (!manifest.contains("bundles"))
        {
            LOG_ERROR("ResourceManager::LoadResourcesFromManifest: Manifest does not contain any bundles");
            return;
        }

        for (const auto& bundle : manifest["bundles"])
        {
            if (!bundle.contains("name") || !bundle.contains("assets"))
            {
                LOG_ERROR("ResourceManager::LoadResourcesFromManifest: Bundle does not contain a name or assets");
                continue;
            }

            const std::string bundleName = bundle["name"];
            const json& bundleAssets = bundle["assets"];

            LOG_INFO("ResourceManager::LoadResourcesFromManifest: Loading bundle: " + bundleName);

            // Load Resources
            for (const auto& asset : bundleAssets)
            {
                if (!asset.contains("name") || !asset.contains("path") || !asset.contains("type"))
                {
                    LOG_ERROR(
                        "ResourceManager::LoadResourcesFromManifest: Asset does not contain a name, path, or type"
                    );
                    continue;
                }

                const std::string assetName = asset["name"];
                const std::string assetPath = asset["path"];
                const std::string assetType = asset["type"];

                LOG_INFO("ResourceManager::LoadResourcesFromManifest: Loading asset: " + assetName);

                if (assetType != "music" && assetType != "shader" && assetType != "sound" && assetType != "texture" &&
                    assetType != "font")
                {
                    LOG_ERROR("ResourceManager::LoadResourcesFromManifest: Invalid asset type: " + assetType);
                    continue;
                }

                if (assetType == "font")
                {
                    auto font = std::make_unique<sf::Font>(assetPath);
                    SetResource<sf::Font>(assetName, std::move(font));
                }
                else if (assetType == "texture")
                {
                    auto texture = std::make_unique<sf::Texture>(assetPath);
                    SetResource<sf::Texture>(assetName, std::move(texture));
                }
                else if (assetType == "music")
                {
                    auto music = std::make_unique<sf::Music>();
                    if (!music->openFromFile(assetPath))
                    {
                        LOG_ERROR(
                            "(ResourceManager::LoadResourcesFromManifest: Error loading music (" + assetName +
                            ") from : " + assetPath
                        );
                    }
                    SetResource<sf::Music>(assetName, std::move(music));
                }
                else if (assetType == "sound")
                {
                }
                else if (assetType == "shader")
                {
                }
            }
        }
    } catch (const nlohmann::json::parse_error& e)
    {
        LOG_ERROR("ResourceManager::LoadResourcesFromManifest: Failed to parse manifest file: " + std::string(e.what()));
    } catch (const std::exception& e)
    {
        LOG_ERROR(
            "(ResourceManager::LoadResourcesFromManifest: An error occurred while loading the manifest file: " +
            std::string(e.what())
        );
    }
}

void ResourceManager::UnloadResource(const std::string& name)
{
    if (const auto it = _resources.find(name); it != _resources.end())
    {
        _resources.erase(name);
    }
}

void ResourceManager::CleanUp()
{
    _resources.clear();
}
