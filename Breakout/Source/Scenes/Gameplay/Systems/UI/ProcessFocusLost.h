// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace GamePlay
{
struct ProcessFocusLost
{
    static void Initialize(const flecs::world& world, const flecs::entity& rootEntity);
};
} // namespace GamePlay
