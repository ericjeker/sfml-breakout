// Copyright (c) Eric Jeker 2025.

#include "DebugModule.h"

#include "Systems/CalculateFpsSystem.h"

namespace Modules
{

DebugModule::DebugModule(const flecs::world& world)
{
    CalculateFpsSystem::Register(world);
}

} // namespace Modules
