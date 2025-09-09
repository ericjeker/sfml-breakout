// Copyright (c) Eric Jeker 2025.

#include "PaddleMovementSystem.h"

#include "Scenes/Gameplay/Components/MoveIntent.h"

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

void PaddleMovementSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const MoveIntent, const Target>("PaddleMovementSystem").kind(flecs::PreUpdate).each(Update).child_of(rootEntity);
}
