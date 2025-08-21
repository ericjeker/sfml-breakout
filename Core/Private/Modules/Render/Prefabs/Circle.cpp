// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Render/Prefabs/Circle.h"

#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

namespace Prefabs
{

flecs::entity Circle::Create(const flecs::world& world, const CircleParams& params)
{
    sf::CircleShape circleShape(params.radius);
    circleShape.setPosition(params.position);
    circleShape.setFillColor(params.color);
    circleShape.setOrigin({params.radius * params.origin.x, params.radius * params.origin.y});

    const auto entity = world.entity()
                            .set<CircleRenderable>({std::move(circleShape)})
                            .set<Transform>({.position = params.position, .scale = params.scale})
                            .set<ZOrder>({params.zOrder});

    return entity;
}


} // namespace Prefabs
