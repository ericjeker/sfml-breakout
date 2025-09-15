// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "Scenes/Gameplay/Components/LaunchBallIntent.h"
#include "Scenes/Gameplay/Components/PauseGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Scenes/Tags/ScenePaused.h"
#include "Core/Utils/Logger.h"

#include <SFML/Window/Keyboard.hpp>

namespace
{

std::function<void(flecs::entity, KeyPressed)> Update(flecs::entity rootEntity)
{
    return [rootEntity](const flecs::entity& e, const KeyPressed& k) {
        if (rootEntity.has<ScenePaused>())
        {
            return;
        }

        bool eventHandled = true;

        if (k.scancode == sf::Keyboard::Scancode::Escape)
        {
            e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>();
        }
        else if (k.scancode == sf::Keyboard::Scancode::Space)
        {
            e.world().entity().add<LifetimeOneFrame>().add<Command>().add<LaunchBallIntent>();
        }
        else
        {
            eventHandled = false;
        }

        if (eventHandled)
        {
            e.destruct();
        }
    };
}

} // namespace

namespace GamePlay
{
void ProcessKeyPressed::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .write<PauseGameIntent>()
        .write<LaunchBallIntent>()
        .each(Update(rootEntity))
        .child_of(rootEntity);
}
} // namespace GamePlay
