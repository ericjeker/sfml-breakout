// Copyright (c) Eric Jeker 2025.


#include "ApplyPaddlePositionToBallSystem.h"

#include "Modules/Breakout/Components/AttachedToPaddle.h"
#include "Modules/Breakout/Components/Ball.h"
#include "Modules/Breakout/Components/Paddle.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"

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

void ApplyPaddlePositionToBallSystem::Register(const flecs::world& world)
{
    world.system<Transform, const AttachedToPaddle, const Ball>("ApplyPaddlePositionToBall")
        .with<GameStatePlaying>()
        .singleton()
        .each(Update);
}
