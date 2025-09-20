// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace Gameplay
{
struct ProcessFocusLost
{
    static void Register(const flecs::world& world, const flecs::entity& rootEntity);
};
} // namespace Gameplay
