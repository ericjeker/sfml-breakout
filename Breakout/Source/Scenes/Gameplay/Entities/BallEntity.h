// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


struct BallEntity
{
    static flecs::entity Create(const flecs::world& world, const flecs::entity& rootEntity, float zOrder);
};
