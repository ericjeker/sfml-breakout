// Copyright (c) Eric Jeker 2025.

#include "ProcessKeyPressed.h"

#include "Scenes/MainMenu/Components/ExitGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/KeyPressed.h"
#include "Core/Utils/Logger.h"

namespace
{

void Update(const flecs::entity& e, const KeyPressed& k)
{
    LOG_DEBUG("ProcessKeyPressed::Update -> KeyPressed");
    if (k.scancode == sf::Keyboard::Scancode::Escape)
    {
        e.world().entity().add<LifetimeOneFrame>().add<Command>().add<ExitGameIntent>();
    }

    e.destruct();
}

} // namespace

namespace MainMenu
{

void ProcessKeyPressed::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    LOG_DEBUG("ProcessKeyPressed::Initialize");
    world.system<const KeyPressed>("ProcessKeyPressed").kind(flecs::PostLoad).each(Update).child_of(rootEntity);
}

}
