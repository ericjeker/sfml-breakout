// Copyright (c) Eric Jeker 2025.

#include "Ball.h"

#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/CircleCollider.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"


namespace Prefabs
{

flecs::entity Ball::Create(const flecs::world& world, const BallParams& params)
{
    // Create a ball
    sf::CircleShape ballShape(params.radius);
    ballShape.setFillColor(params.color);
    ballShape.setOrigin({params.radius, params.radius});

    return world.entity()
        .set<Acceleration>({params.acceleration})
        .set<CircleRenderable>({std::move(ballShape)})
        .set<Friction>({params.friction})
        .set<Gravity>({params.gravity})
        .set<Transform>({.position = params.position})
        .set<Velocity>({params.velocity})
        .set<CircleCollider>({params.radius})
        .set<ZOrder>({params.zOrder});
}

} // namespace Prefabs
