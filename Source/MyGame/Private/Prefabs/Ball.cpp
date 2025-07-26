// Copyright (c) Eric Jeker 2025.

#include "Ball.h"

#include "../Components/BallRenderable.h"
#include "Components/Physics/RigidBody.h"
#include "Components/Physics/Velocity.h"
#include "Components/Transform/Transform.h"
#include "PhysicsConstants.h"

Ball::Ball(const flecs::world& world)
{
    world.prefab("Ball")
        .set<Transform>({})
        .set<Velocity>({})
        .set<RigidBody>({.damping = PhysicsConstants::NO_DAMPING})
        .set<BallRenderable>({});
}
