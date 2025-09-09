// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "Scenes/Gameplay/Components/LaunchBallIntent.h"
#include "Scenes/Gameplay/Components/PauseGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Utils/Logger.h"

#include <SFML/Window/Keyboard.hpp>

namespace
{

void Update(const flecs::entity& e, const KeyPressed& k)
{
    LOG_DEBUG("GameplayScene::ProcessKeyPressed");

    if (k.scancode == sf::Keyboard::Scancode::Escape)
    {
        LOG_DEBUG("GameplayScene::ProcessKeyPressed: Escape -> Add PauseGameIntent");
        e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>();
    }
    else if (k.scancode == sf::Keyboard::Scancode::Space)
    {
        e.world().entity().add<LifetimeOneFrame>().add<Command>().add<LaunchBallIntent>();
    }

    e.destruct();
}

} // namespace

void ProcessKeyPressed::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .write<PauseGameIntent>()
        .write<LaunchBallIntent>()
        // Make sure we process only the KeyPressed events from this scene
        .with(flecs::ChildOf, rootEntity)
        .each(Update)
        .child_of(rootEntity);
}
