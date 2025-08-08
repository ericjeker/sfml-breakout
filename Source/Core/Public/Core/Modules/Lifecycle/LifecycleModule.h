// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>

namespace Modules
{

struct LifecycleModule
{
    explicit LifecycleModule(const flecs::world& world);
};

} // namespace Modules
