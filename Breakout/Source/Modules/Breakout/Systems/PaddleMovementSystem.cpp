// Copyright (c) Eric Jeker 2025.

#include "PaddleMovementSystem.h"

#include "Modules/Breakout/Components/MoveIntent.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Input/Components/Target.h"
#include "Core/Modules/Physics/Components/Acceleration.h"

namespace
{

void Update(const flecs::entity& cmd, const MoveIntent& i, const Target& t)
{
    auto& [acceleration] = t.entity.get_mut<Acceleration>();
    acceleration.y += i.accelerate.y * 1000.f;
    acceleration.x += i.accelerate.x * 1000.f;

    // Destroy the command entity
    cmd.destruct();
}

} // namespace

void PaddleMovementSystem::Register(const flecs::world& world)
{
    world.system<const MoveIntent, const Target>("PaddleMovementSystem")
        .kind(flecs::PreUpdate)
        .read<Command>()
        .with<GameStatePlaying>()
        .singleton()
        .each(Update);
}
