// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Camera/CameraModule.h"

#include "Core/GameService.h"
#include "Core/Modules/Camera/Components/CameraShake.h"
#include "Core/Modules/Camera/Components/CameraShakeIntent.h"
#include "Core/Modules/Camera/Singletons/MainCamera.h"
#include "Core/Modules/Window/Components/WindowResizeIntent.h"
#include "Core/Modules/Window/Singletons/WindowSize.h"
#include "Core/Utils/Logger.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <flecs.h>

namespace
{

void UpdateViewport(const flecs::entity e, const WindowResizeIntent& intent)
{
    LOG_DEBUG("CameraModule::UpdateViewport -> Window resized, updating viewport");
    const auto& [currentSize, refSize] = e.world().get<WindowSize>();

    // Calculate scale to maintain aspect ratio
    const float windowAspectRatio = static_cast<float>(currentSize.x) / static_cast<float>(currentSize.y);
    const float gameAspectRatio = static_cast<float>(refSize.x) / static_cast<float>(refSize.y);

    sf::FloatRect viewport;

    if (windowAspectRatio > gameAspectRatio)
    {
        // Window is wider than the game: add pillarbox (black bars on the sides)
        viewport.size.x = gameAspectRatio / windowAspectRatio;
        viewport.size.y = 1.0f;
        viewport.position.x = (1.0f - viewport.size.x) / 2.0f;
        viewport.position.y = 0.0f;
    }
    else
    {
        // Window is taller than the game: add letterbox (black bars on top/bottom)
        viewport.size.x = 1.0f;
        viewport.size.y = windowAspectRatio / gameAspectRatio;
        viewport.position.x = 0.0f;
        viewport.position.y = (1.0f - viewport.size.y) / 2.0f;
    }

    // Update the main camera viewport
    e.world().get_mut<MainCamera>().view.setViewport(viewport);
}

void ProcessCameraShakeIntent(const flecs::entity& e, const CameraShakeIntent& intent)
{
    const auto& world = e.world();

    bool existingShake = false;
    world.query<CameraShake>().each([&existingShake, intent](CameraShake& shake) {
        // Shake is already in progress, ignore the new intent
        existingShake = true;
        shake.intensity = std::max(shake.intensity, intent.intensity);
        shake.remainingTime = std::max(shake.remainingTime, intent.duration);
    });

    // If a shake is already in progress, ignore the new intent
    if (!existingShake)
    {
        // Start the shake by adding the CameraShake component
        const auto& view = world.get<MainCamera>().view;
        const auto currentCenter = view.getCenter();

        // Add or update the shake component on a dedicated entity
        world.entity("CameraShakeEntity")
            .set<CameraShake>(
                {.intensity = intent.intensity,
                 .remainingTime = intent.duration,
                 .originalCenter = currentCenter,
                 .currentOffset = {0.f, 0.f}}
            );
    }

    // Remove the intent (it's been processed)
    e.destruct();
}

void UpdateCameraShake(const flecs::entity& e, CameraShake& shake)
{
    shake.remainingTime -= e.world().delta_time();

    auto& [view] = e.world().get_mut<MainCamera>();
    if (shake.remainingTime <= 0.f)
    {
        // Re-center the camera properly
        view.setCenter(shake.originalCenter);
        e.destruct();
        return;
    }

    // Generate random shake offset
    const float shakeX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shake.intensity;
    const float shakeY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shake.intensity;
    shake.currentOffset = {shakeX, shakeY};

    // Apply shake to camera
    view.setCenter(shake.originalCenter + shake.currentOffset);
}

} // namespace


namespace Core::Modules
{

CameraModule::CameraModule(const flecs::world& world)
{
    // --- Initialize the main camera ---
    sf::View cameraView;
    assert(world.has<WindowSize>());
    const auto& [currentSize, refSize] = world.get<WindowSize>();
    cameraView.setSize({static_cast<float>(refSize.x), static_cast<float>(refSize.y)});
    cameraView.setCenter({static_cast<float>(refSize.x) / 2.f, static_cast<float>(refSize.y) / 2.f});

    // --- Declare Singletons ---
    world.set<MainCamera>({.view = std::move(cameraView)});

    // --- Declare Systems ---
    world.system<const WindowResizeIntent>("UpdateViewport").kind(flecs::PostLoad).each(UpdateViewport);
    world.system<const CameraShakeIntent>("ProcessCameraShakeIntent").each(ProcessCameraShakeIntent);
    world.system("ApplyCameraToWindow").kind(flecs::PreStore).run([](const flecs::iter& i) {
        GameService::Get<sf::RenderWindow>().setView(i.world().get<MainCamera>().view);
    });
    world.system<CameraShake>("UpdateCameraShake").kind(flecs::PreStore).each(UpdateCameraShake);
}

} // namespace Modules
