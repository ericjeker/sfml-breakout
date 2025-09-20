// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Input/InputModule.h"

#include "Core/Modules/Input/Components/CommandQueue.h"
#include "Core/Modules/Input/Components/PossessedByPlayer.h"
#include "Core/Modules/Input/Components/Target.h"
#include "Core/Modules/Input/Singletons/InputBindings.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"

namespace Core::Modules
{

InputModule::InputModule(const flecs::world& world)
{
    world.component<PossessedByPlayer>();
    world.component<Command>();

    world.singleton<InputBindings>();

    world.system<const InputBindings>("InputSystem")
        .term_at(0)
        .singleton()
        .kind(flecs::PostLoad)
        // If we don't specify we write commands then we risk having a frame lag
        .write<Command>()
        .each([&](const flecs::iter& it, size_t, const InputBindings& b) {
            // Query the player currently possessed
            const auto q = it.world().query<const PossessedByPlayer>();
            if (q.count() == 0)
            {
                return;
            }

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
                q.each([&](flecs::entity e, const PossessedByPlayer& p) {
                    it.world().entity().is_a(prefab).add<LifetimeOneFrame>().add<Command>().add<Target>().set<Target>({e});
                });
            }
        });
}

} // namespace Modules
