// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Lifetime/LifetimeModule.h"

#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"

namespace
{

void UpdateLifetime(const flecs::iter& it, size_t, Lifetime& lifetime)
{
    lifetime.seconds -= it.delta_time();
}

void LifetimeCull(const flecs::entity& e, const Lifetime& lifetime)
{
    if (lifetime.seconds <= 0.f)
    {
        e.destruct();
    }
}

/**
 * Destroys the given entity.
 *
 * @param e The flecs::entity object that needs to be destroyed.
 */
void DestroyEntity(const flecs::entity& e, const LifetimeOneFrame&)
{
    e.destruct();
}

} // namespace

namespace Core::Modules
{

LifetimeModule::LifetimeModule(const flecs::world& world)
{
    world.component<LifetimeOneFrame>();
    world.component<Lifetime>();

    world.system<Lifetime>("LifetimeUpdateSystem").kind(flecs::OnUpdate).each(UpdateLifetime);
    world.system<const Lifetime>("LifetimeCullSystem").kind(flecs::PostUpdate).each(LifetimeCull);
    world.system<const LifetimeOneFrame>("LifetimeOneFrameSystem").kind(flecs::PostUpdate).each(DestroyEntity);
}

} // namespace Modules
