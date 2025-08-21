// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Scene/SceneModule.h"

#include "Core/Modules/Scene/Components/ScenePaused.h"


namespace Modules
{

SceneModule::SceneModule(const flecs::world& world)
{
    // Tell the world that ScenePause can be toggled on and off
    world.component<ScenePaused>().add(flecs::CanToggle);
}

} // namespace Modules
