// Copyright (c) Eric Jeker 2025.

#include "Text.h"

#include "Components/Transform.h"
#include "Modules/Render/Components/TextRenderable.h"

namespace Prefabs
{

flecs::entity Text::Create(const flecs::world& world, const TextParams& params)
{
    sf::Text text(*params.font, params.text, params.size);
    text.setFillColor(params.color);
    text.setOrigin(params.origin);
    text.setPosition(params.position);
    text.setScale(params.scale);

    const auto entity = world.entity()
                            .set<TextRenderable>({std::move(text)})
                            .set<Transform>({.position = params.position, .scale = params.scale, .rotation = params.rotation});

    return entity;
}


} // namespace Prefabs
