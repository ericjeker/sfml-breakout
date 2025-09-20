// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/UI/Prefabs/Text.h"

#include "Core/GameService.h"

#include "Core/Managers/ResourceManager.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

namespace Prefabs
{

flecs::entity Text::Create(const flecs::world& world, const TextParams& params)
{
    const auto font = GameService::Get<ResourceManager>().GetResource<sf::Font>(params.fontAsset);
    if (!font)
    {
        return flecs::entity::null();
    }

    auto text = std::make_unique<sf::Text>(*font, params.text, params.fontSize);
    text->setFillColor(params.textColor);
    const sf::FloatRect textBounds = text->getLocalBounds();
    text->setOrigin(
        {textBounds.size.x * params.origin.x + textBounds.position.x,
         textBounds.size.y * params.origin.y + textBounds.position.y}
    );
    text->setPosition(params.position);
    text->setScale(params.scale);

    const auto entity = world.entity()
                            .set<TextRenderable>({.text = std::move(text)})
                            .set<Origin>({params.origin})
                            .set<ZOrder>({params.zOrder})
                            .set<Transform>({.position = params.position, .scale = params.scale, .rotation = params.rotation});

    return entity;
}


} // namespace Prefabs
