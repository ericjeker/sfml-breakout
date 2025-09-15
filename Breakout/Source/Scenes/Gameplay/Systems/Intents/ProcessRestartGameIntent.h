// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


struct ProcessRestartGameIntent
{
    static void Initialize(const flecs::world& world, const flecs::entity& rootEntity);
};
