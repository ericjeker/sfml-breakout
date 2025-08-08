// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Lifecycle/LifecycleModule.h"

#include "Core/Modules/Lifecycle/Components/LifetimeOneFrame.h"


namespace
{

/**
 * Destroys the given entity.
 *
 * @param e The flecs::entity object that needs to be destroyed.
 * @param lifetime The LifetimeOneFrame instance, indicating the lifetime context for the entity.
 */
void DestroyEntity(const flecs::entity e, const LifetimeOneFrame& lifetime)
{
    e.destruct();
}

} // namespace

namespace Modules
{

LifecycleModule::LifecycleModule(const flecs::world& world)
{
    world.component<LifetimeOneFrame>();
    world.system<const LifetimeOneFrame>("LifetimeSystem").kind(flecs::PostUpdate).each(DestroyEntity);
}

} // namespace Modules
