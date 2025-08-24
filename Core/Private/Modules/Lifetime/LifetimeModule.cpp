// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Lifetime/LifetimeModule.h"

#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"


namespace
{

void UpdateLifetime(const flecs::iter& it, size_t, Lifetime& lifetime)
{
    lifetime.lifetime -= it.delta_time();
}

void LifetimeCull(const flecs::entity e, const Lifetime& lifetime)
{
    if (lifetime.lifetime <= 0.f)
    {
        e.destruct();
    }
}

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

LifetimeModule::LifetimeModule(const flecs::world& world)
{
    world.component<LifetimeOneFrame>();
    world.component<Lifetime>();

    world.system<Lifetime>("LifetimeUpdateSystem").kind(flecs::OnUpdate).each(UpdateLifetime);
    world.system<const Lifetime>("LifetimeCullSystem").kind(flecs::PostUpdate).each(LifetimeCull);
    world.system<const LifetimeOneFrame>("LifetimeSystem").kind(flecs::PostUpdate).each(DestroyEntity);
}

} // namespace Modules
