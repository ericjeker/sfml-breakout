// Copyright (c) Eric Jeker 2025.

#include "ApplyPaddlePositionToBallSystem.h"

#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Scenes/Gameplay/Components/AttachedToPaddle.h"
#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/Paddle.h"

#include "Core/Modules/Render/Components/Transform.h"

namespace
{

void Update(const flecs::entity& e, Transform& t, const AttachedToPaddle& ap, const Ball& b)
{
    // Get the paddle
    e.world().query<const Transform, const Paddle>().each(
        [&ballTransform = t](const Transform& paddleTransform, const Paddle& p) {
            ballTransform.position.x = paddleTransform.position.x;
        }
    );
}

} // namespace

void ApplyPaddlePositionToBallSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<Transform, const AttachedToPaddle, const Ball>("ApplyPaddlePositionToBall")
        .with<GameStatePlaying>().singleton()
        .each(Update).child_of(rootEntity);
}
