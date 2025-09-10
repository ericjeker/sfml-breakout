// Copyright (c) Eric Jeker 2025.

#include "ProcessFocusLost.h"

#include "Scenes/Gameplay/Components/PauseGameIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/UI/Components/FocusLost.h"

namespace
{

void Update(const flecs::entity e, const FocusLost f)
{
    e.world().entity().add<LifetimeOneFrame>().add<Command>().add<PauseGameIntent>();
}

} // namespace

namespace GamePlay
{
void ProcessFocusLost::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const FocusLost>().write<PauseGameIntent>().each(Update).child_of(rootEntity);
}
} // namespace GamePlay
