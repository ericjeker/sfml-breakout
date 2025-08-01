// Copyright (c) Eric Jeker 2025.

#include "Button.h"

#include "../../Physics/Components/Transform.h"
#include "Components/EventTrigger.h"
#include "Components/Size.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Render/Components/RectangleRenderable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/Render/Components/ZOrder.h"
#include "Modules/UI/Components/ButtonBackground.h"
#include "Modules/UI/Components/ButtonText.h"
#include "Modules/UI/Components/Clickable.h"
#include "Modules/UI/Components/Hoverable.h"


namespace Prefabs
{

flecs::entity Button::Create(const flecs::world& world, const ButtonParams& params)
{
    const auto font = GameService::Get<ResourceManager>().GetResource<sf::Font>(params.fontAsset);
    if (!font)
    {
        return flecs::entity::null();
    }

    // --- Create text ---
    auto buttonText = std::make_unique<sf::Text>(*font, params.text, params.fontSize);
    const sf::FloatRect textBounds = buttonText->getLocalBounds();
    const float originX = textBounds.size.x * params.origin.x + textBounds.position.x;
    const float originY = textBounds.size.y * params.origin.y + textBounds.position.y;

    buttonText->setFillColor(params.textColor);
    buttonText->setOrigin({originX, originY});
    buttonText->setPosition(params.position);

    // --- Create background ---
    sf::RectangleShape buttonBackground;
    buttonBackground.setSize({textBounds.size.x + params.padding.x * 2, textBounds.size.y + params.padding.y * 2});
    const sf::Vector2f size = buttonBackground.getSize();
    buttonBackground.setOrigin({size.x * params.origin.x, size.y * params.origin.y});
    buttonBackground.setFillColor(params.backgroundColor);
    buttonBackground.setPosition(params.position);

    // --- Create the entity using the prefab ---
    const flecs::entity
        entity = world.entity()
                     .set<Transform>({.position = params.position})
                     .set<Size>({buttonBackground.getSize()})
                     .set<TextRenderable>({.text = std::move(buttonText)})
                     .set<RectangleRenderable>({.shape = std::move(buttonBackground)})
                     .set<ZOrder>({params.zOrder})
                     .set<ButtonBackground>(
                         {.backgroundColor = params.backgroundColor, .hoverColor = params.hoverColor, .padding = params.padding}
                     )
                     .set<ButtonText>(
                         {.text = params.text,
                          .fontSize = params.fontSize,
                          .hoverColor = params.textHoverColor,
                          .textColor = params.textColor}
                     )
                     .set<Clickable>({})
                     .set<Hoverable>({});

    if (params.onClick)
    {
        entity.set<EventTrigger>({.callback = params.onClick});
    }

    return entity;
}


} // namespace Prefabs
