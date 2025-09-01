// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Render/Factories/Rectangle.h"

#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/RectangleRenderable.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

namespace Factories
{

flecs::entity Rectangle::Create(const flecs::world& world, const RectangleParams& params)
{
    sf::RectangleShape backgroundDrawable;
    backgroundDrawable.setSize(params.size);
    backgroundDrawable.setPosition(params.position);
    backgroundDrawable.setOrigin(params.size.componentWiseMul(params.origin));
    backgroundDrawable.setFillColor(params.color);

    const auto entity = world.entity()
                            .set<RectangleRenderable>({std::move(backgroundDrawable)})
                            .set<ZOrder>({params.zOrder})
                            .set<Origin>({params.origin})
                            .set<Size>({params.size})
                            .set<Transform>({.position = params.position, .scale = params.scale, .rotation = params.rotation});

    return entity;
}

} // namespace Factories
