// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "Modules/Breakout/Components/Intents/LaunchBallIntent.h"
#include "Modules/Breakout/Components/Intents/PauseGameIntent.h"
#include "Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Scene/Components/ActiveScene.h"
#include "Core/Modules/Scene/Tags/ScenePaused.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Utils/Logger.h"

#include <SFML/Window/Keyboard.hpp>

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const KeyPressed& k) {
        if (const auto& [rootEntity] = e.world().get<ActiveScene>(); rootEntity.has<ScenePaused>())
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
void ProcessKeyPressed::Register(const flecs::world& world)
{
    world.system<const KeyPressed>("Gameplay.ProcessKeyPressed")
        .kind(flecs::PreUpdate)
        .write<PauseGameIntent>()
        .write<LaunchBallIntent>()
        .with<GameStatePlaying>()
        .singleton()
        .each(Update());
}
} // namespace Gameplay
