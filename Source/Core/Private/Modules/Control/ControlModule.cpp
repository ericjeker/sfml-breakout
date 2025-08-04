// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Control/ControlModule.h"

#include "Core/Modules/Control/Components/CommandQueue.h"
#include "Core/Modules/Control/Components/LifetimeOneFrame.h"
#include "Core/Modules/Control/Components/PossessedByPlayer.h"

namespace
{

void DestroyEntity(const flecs::entity e, const LifetimeOneFrame& lifetime)
{
    e.destruct();
}

} // namespace


namespace Modules
{

ControlModule::ControlModule(const flecs::world& world)
{
    world.component<PossessedByPlayer>();
    world.component<CommandQueue>();
    world.component<Command>();
    world.component<LifetimeOneFrame>();


    world.system<const LifetimeOneFrame>().each(DestroyEntity);
}


} // namespace Modules
