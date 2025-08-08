// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Control/ControlModule.h"

#include "Core/Logger.h"
#include "Core/Modules/Control/Components/CommandQueue.h"
#include "Core/Modules/Control/Components/LifetimeOneFrame.h"
#include "Core/Modules/Control/Components/PossessedByPlayer.h"
#include "Core/Modules/Control/Components/Target.h"
#include "Core/Modules/Control/Singletons/InputBindings.h"


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

    world.singleton<InputBindings>();

    world.system<const LifetimeOneFrame>("LifetimeSystem").kind(flecs::PostUpdate).each(DestroyEntity);
    world.system<const InputBindings>("InputSystem")
        .term_at(0)
        .singleton()
        .kind(flecs::PostLoad)
        .each(
            [&](const flecs::iter& it, size_t, const InputBindings& b)
            {
                const auto q = it.world().query<const PossessedByPlayer>();

                // Loop each binding to see if the input is activated
                for (const auto& [inputKey, prefab] : b.map)
                {
                    if (!inputKey.isActivated())
                    {
                        continue;
                    }

                    // TODO:
                    //   - Add the Command as child_of the entity
                    //   - Add a Seq number to guarantee the sequence of commands
                    LOG_DEBUG("(ControlModule::InputSystem): InputKey is activated, adding prefab.");
                    q.each([&](flecs::entity e, const PossessedByPlayer& p)
                           { it.world().entity().is_a(prefab).add<LifetimeOneFrame>().add<Target>().set<Target>({e}); });
                }
            }
        );
}


} // namespace Modules
