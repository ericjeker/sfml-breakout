// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>

namespace Core::Modules
{

/**
 * @brief The event module is responsible for processing the polled discrete events.
 *
 * Polled events are generated early in the gaming loop, they are polled by SFML and triggered only once. The
 * event module will process those events and check if they have a corresponding binding. The binding will generate
 * an Intent entity in the world that is later processed by another system.
 *
 * A similar system is the InputModule that handles the player control and requires possession.
 */
struct EventModule
{
    explicit EventModule(const flecs::world& world);
};

}
