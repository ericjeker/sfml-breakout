// Copyright (c) Eric Jeker 2025.
#include "Core/Managers/SceneManager.h"

#include <SFML/Graphics/Font.hpp>

#include <ranges>
#include <tracy/Tracy.hpp>

void SceneManager::UnloadAll() const
{
    ZoneScopedN("SceneManager::UnloadAll");
    for (const auto& typeIdx : _sceneOrder)
    {
        const auto& scene = _scenes.at(typeIdx);
        scene->SetLoaded(false);
        scene->Shutdown();
    }
}
