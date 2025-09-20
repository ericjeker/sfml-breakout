// Copyright (c) Eric Jeker 2025.

#include "ProcessFocusLost.h"

#include "Modules/Breakout/Components/Intents/PauseGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/FocusLost.h"

namespace
{

auto Update()
{
    return [](const flecs::entity e, const FocusLost f) {
        e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>();
    };
}

} // namespace

// TODO: this sucks, change it!
namespace Gameplay
{

void ProcessFocusLost::Register(const flecs::world& world)
{
    world.system<const FocusLost>().write<PauseGameIntent>().each(Update());
}

} // namespace Gameplay
