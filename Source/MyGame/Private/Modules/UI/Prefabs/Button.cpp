// Copyright (c) Eric Jeker 2025.

#include "Button.h"

#include "Components/EventTrigger.h"
#include "Components/Size.h"
#include "Managers/GameService.h"
#include "Managers/ResourceManager.h"
#include "Modules/Physics/Components/Transform.h"
#include "Modules/Render/Components/RectangleRenderable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Modules/Render/Components/ZOrder.h"
#include "Modules/UI/Components/ButtonBackground.h"
#include "Modules/UI/Components/ButtonText.h"
#include "Modules/UI/Components/Clickable.h"
#include "Modules/UI/Components/Hoverable.h"
#include "Modules/UI/Components/Interactable.h"


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

    // --- Create the main reactive zone entity ---
    const sf::FloatRect buttonBounds = buttonBackground.getGlobalBounds();
    const flecs::entity buttonEntity = world.entity()
                                           .set<Interactable>({})
                                           .set<Transform>({.position = buttonBounds.position})
                                           .set<Size>({buttonBounds.size})
                                           .set<ZOrder>({params.zOrder}) // Base Z-order
                                           .set<Clickable>({});

    if (params.onClick)
    {
        buttonEntity.set<EventTrigger>({.callback = params.onClick});
    }

    // --- Create the background entity as a child of the reactive zone ---
    const flecs::entity backgroundEntity = world.entity()
                                               .child_of(buttonEntity)
                                               .set<Transform>({.position = params.position})
                                               .set<RectangleRenderable>({.shape = std::move(buttonBackground)})
                                               .set<ButtonBackground>(
                                                   {.backgroundColor = params.backgroundColor,
                                                    .hoverColor = params.hoverColor,
                                                    .padding = params.padding}
                                               )
                                               .set<ZOrder>({params.zOrder + 0.1f}); // Increment Z-order

    // --- Create the text entity as a child of the background entity ---
    world.entity()
        .child_of(backgroundEntity)
        .set<Transform>({.position = params.position})
        .set<TextRenderable>({.text = std::move(buttonText)})
        .set<ButtonText>(
            {.text = params.text, .fontSize = params.fontSize, .hoverColor = params.textHoverColor, .textColor = params.textColor}
        )
        .set<ZOrder>({params.zOrder + 0.2f}); // Further increment Z-order

    return buttonEntity;
}


} // namespace Prefabs
