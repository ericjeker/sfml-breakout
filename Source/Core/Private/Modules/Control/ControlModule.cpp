// Copyright (c) Eric Jeker 2025.

#include "Modules/Control/ControlModule.h"

#include "Modules/Control/Components/ActionRegistry.h"
#include "Modules/Control/Components/PossessedByPlayer.h"

namespace Modules
{

ControlModule::ControlModule(const flecs::world& world)
{
    world.component<PossessedByPlayer>();
    world.component<ActionRegistry>();
}


} // namespace Modules
