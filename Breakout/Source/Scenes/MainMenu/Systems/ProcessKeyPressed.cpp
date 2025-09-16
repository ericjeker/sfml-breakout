// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "Modules/Breakout/Singletons/GameStateMenu.h"
#include "Scenes/MainMenu/Components/ExitGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Utils/Logger.h"

namespace
{

void Update(const flecs::entity& e, const KeyPressed& k)
{
    LOG_DEBUG("MainMenu::ProcessKeyPressed::Update -> KeyPressed");
    if (k.scancode == sf::Keyboard::Scancode::Escape)
    {
        e.world().entity().add<LifetimeOneFrame>().add<Command>().add<ExitGameIntent>();
    }

    LOG_DEBUG("MainMenu::ProcessKeyPressed: Key Handled, destroying.");
    e.destruct();
}

} // namespace

namespace MainMenu
{

void ProcessKeyPressed::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const KeyPressed>("ProcessKeyPressed")
        .kind(flecs::PostLoad)
        .write<ExitGameIntent>()
        .with<GameStateMenu>().singleton()
        .each(Update)
        .child_of(rootEntity);
}

} // namespace MainMenu
