// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>

namespace Modules
{

struct DebugModule
{
    explicit DebugModule(const flecs::world& world);
};

} // namespace Modules
