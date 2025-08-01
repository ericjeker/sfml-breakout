// Copyright (c) Eric Jeker 2025.

#include "Text.h"

#include "../../Physics/Components/Transform.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Render/Components/TextRenderable.h"

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
                            .set<Transform>({.position = params.position, .scale = params.scale, .rotation = params.rotation});

    return entity;
}


} // namespace Prefabs
