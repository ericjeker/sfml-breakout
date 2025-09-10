// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


namespace MainMenu
{
struct ProcessStartGameIntent
{
    static void Initialize(const flecs::world& world, const flecs::entity& rootEntity);
};
} // namespace MainMenu
