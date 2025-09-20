// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "../../Components/Intents/LaunchBallIntent.h"
#include "../../Components/Intents/PauseGameIntent.h"
#include "Modules/Breakout/Components/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Scenes/Tags/ScenePaused.h"
#include "Core/Utils/Logger.h"

#include <SFML/Window/Keyboard.hpp>

namespace
{

auto Update(flecs::entity rootEntity)
{
    return [rootEntity](const flecs::entity& e, const KeyPressed& k) {
        if (rootEntity.has<ScenePaused>())
        {
            LOG_DEBUG("Gameplay::ProcessKeyPressed -> Scene is paused, ignoring key");
            return;
        }

        bool eventHandled = true;

        if (k.scancode == sf::Keyboard::Scancode::Escape)
        {
            LOG_DEBUG("Gameplay::ProcessKeyPressed -> Escape -> Pause Game");
            e.world().entity().set<TransitionGameStateIntent>({GameTransitions::PauseGame});
        }
        else if (k.scancode == sf::Keyboard::Scancode::Space)
        {
            LOG_DEBUG("Gameplay::ProcessKeyPressed -> Space -> Launch Ball");
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

namespace Gameplay
{
void ProcessKeyPressed::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const KeyPressed>("Gameplay.ProcessKeyPressed")
        .kind(flecs::PreUpdate)
        .write<PauseGameIntent>()
        .write<LaunchBallIntent>()
        .with<GameStatePlaying>()
        .singleton()
        .each(Update(rootEntity))
        .child_of(rootEntity);
}
} // namespace Gameplay
