// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Render/Factories/Circle.h"

#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

namespace Factories
{

flecs::entity Circle::Create(const flecs::world& world, const CircleParams& params)
{
    sf::CircleShape circleShape(params.radius);
    circleShape.setPosition(params.position);
    circleShape.setFillColor(params.color);
    circleShape.setOrigin({2 * params.radius * params.origin.x, 2 * params.radius * params.origin.y});

    const auto entity = world.entity()
                            .set<CircleRenderable>({std::move(circleShape)})
                            .set<Transform>({.position = params.position, .scale = params.scale})
                            .set<ZOrder>({params.zOrder})
                            .set<Origin>({params.origin})
                            .set<Radius>({params.radius});

    return entity;
}


} // namespace Factories
