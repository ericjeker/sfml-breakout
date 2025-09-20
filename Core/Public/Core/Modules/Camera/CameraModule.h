// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace Core::Modules
{

/**
 * @brief Integrates camera-related functionality into the Flecs world.
 *
 * The CameraModule is responsible for initializing and configuring camera systems
 * and singletons within the Flecs world. It serves as a module to enable
 * camera handling and management in the game engine's environment.
 *
 * @param world A constant reference to a Flecs world object where the camera
 *              module and its related systems will be imported.
 */
struct CameraModule
{
    explicit CameraModule(const flecs::world& world);
};

}
