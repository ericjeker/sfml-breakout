// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>


struct OutOfBoundsSystem
{
    static void Register(const flecs::world& world);
};
