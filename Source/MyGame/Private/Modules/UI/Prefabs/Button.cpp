// Copyright (c) Eric Jeker 2025.

#include "Button.h"

#include "Components/EventTrigger.h"
#include "Components/Size.h"
#include "Components/Transform.h"
#include "Managers/GameService.h"
#include "Modules/Render/Components/RectangleRenderable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/UI/Components/ButtonBackground.h"
#include "Modules/UI/Components/ButtonText.h"
#include "Modules/UI/Components/Clickable.h"
#include "Modules/UI/Components/Hoverable.h"


namespace Prefabs
{

flecs::entity Button::Create(const flecs::world& world, const ButtonParams& params)
{
    // --- Create text ---
    sf::Text buttonText(*params.font, params.text, params.fontSize);
    const sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setFillColor(params.textColor);
    buttonText.setOrigin(textBounds.size / 2.f);

    // --- Create background ---
    sf::RectangleShape buttonBackground;
    buttonBackground.setSize({textBounds.size.y + params.padding.y * 2, textBounds.size.x + params.padding.x * 2});
    buttonBackground.setOrigin(buttonBackground.getSize() / 2.f);
    buttonBackground.setFillColor(params.backgroundColor);

    // --- Create the entity using the prefab ---
    const flecs::entity
        entity = world.entity()
                     .set<Transform>({.position = {0.f, 0.f}})
                     .set<Size>({buttonBackground.getSize()})
                     .set<TextRenderable>({.text = std::move(buttonText)})
                     .set<RectangleRenderable>({.shape = std::move(buttonBackground)})
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
