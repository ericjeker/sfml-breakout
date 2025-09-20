// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


struct ProcessContinueGameIntent
{
    static void Register(const flecs::world& world, const flecs::entity& rootEntity);
};
