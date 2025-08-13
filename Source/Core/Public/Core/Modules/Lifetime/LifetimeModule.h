// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>

namespace Modules
{

struct LifetimeModule
{
    explicit LifetimeModule(const flecs::world& world);
};

} // namespace Modules
