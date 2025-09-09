// Copyright (c) Eric Jeker 2025.

#include "ProcessLaunchBallIntent.h"

#include "Scenes/Gameplay/Components/AttachedToPaddle.h"
#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/LaunchBallIntent.h"

#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Velocity.h"

namespace
{

void Update(const flecs::entity& cmd, const LaunchBallIntent& l)
{
    // Get the ball entity
    cmd.world().query<const Ball, const AttachedToPaddle>().each(
        [](const flecs::entity ball, const Ball b, const AttachedToPaddle ap) {
            ball.remove<AttachedToPaddle>();
            ball.set<Velocity>({{0.f, -500.f}});
            ball.set<ColliderShape>({Shape::Circle});
        }
    );

    // Destroy the command entity
    cmd.destruct();
}

} // namespace

void ProcessLaunchBallIntent::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const LaunchBallIntent>("LaunchBallSystem").kind(flecs::PreUpdate).each(Update).child_of(rootEntity);
}
