// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace Menu
{
struct ProcessExitGameIntent
{
    static void Initialize(const flecs::world& world, const flecs::entity& rootEntity);
};
} // namespace Menu
