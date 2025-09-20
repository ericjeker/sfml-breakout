// Copyright (c) Eric Jeker 2025.

#include "UpdateMultiplierWidget.h"

#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Tags/MultiplierWidget.h"

#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/TextRenderable.h"

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const TextRenderable& t) {
        const auto& [multiplier] = e.world().get<Multiplier>();
        const auto& [origin] = e.get<Origin>();

        t.text->setString("MULT " + std::to_string(multiplier) + "x");

        // Update the origin based on the text bounds
        const sf::FloatRect textBounds = t.text->getLocalBounds();
        t.text->setOrigin(
            {textBounds.size.x * origin.x + textBounds.position.x, textBounds.size.y * origin.y + textBounds.position.y}
        );
    };
}

} // namespace

void UpdateMultiplierWidget::Register(const flecs::world& world)
{
    world.system<TextRenderable>("UpdateMultiplierWidget")
        .with<GameStatePlaying>()
        .singleton()
        .with<MultiplierWidget>()
        .kind(flecs::PostUpdate)
        .each(Update());
}
