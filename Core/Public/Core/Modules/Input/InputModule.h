// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace Core::Modules
{

/**
 * @brief The input module is a controller for a possessed entity.
 *
 * The input module is responsible for handling input events and updating the possessed entity's state accordingly.
 * This works by adding Intent entities in the world whenever the input is triggered. The intent is then processed by
 * its own system.
 *
 * A similar module is the EventModule that handles polled events and doesn't require possession.
 */
struct InputModule
{
    explicit InputModule(const flecs::world& world);
};

} // namespace Core::Modules
