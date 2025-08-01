// Copyright (c) Eric Jeker 2025.

#include "Ball.h"

#include "../../Physics/Components/Transform.h"
#include "Modules/Physics/Components/Acceleration.h"
#include "Modules/Physics/Components/Collider.h"
#include "Modules/Physics/Components/Friction.h"
#include "Modules/Physics/Components/Gravity.h"
#include "Modules/Physics/Components/Velocity.h"
#include "Modules/Render/Components/CircleRenderable.h"
#include "Modules/Render/Components/ZOrder.h"


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
        .set<Collider>({params.radius})
        .set<ZOrder>({params.zOrder});
}

} // namespace Prefabs
