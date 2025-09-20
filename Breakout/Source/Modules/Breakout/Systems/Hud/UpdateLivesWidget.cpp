// Copyright (c) Eric Jeker 2025.

#include "UpdateLivesWidget.h"

#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/Lives.h"
#include "Modules/Breakout/Tags/LivesWidget.h"

#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/TextRenderable.h"

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const TextRenderable& t) {
        const auto& [lives] = e.world().get<Lives>();
        const auto& [origin] = e.get<Origin>();

        t.text->setString("LIVES " + std::to_string(lives));

        // Update the origin based on the text bounds
        const sf::FloatRect textBounds = t.text->getLocalBounds();
        t.text->setOrigin(
            {textBounds.size.x * origin.x + textBounds.position.x, textBounds.size.y * origin.y + textBounds.position.y}
        );
    };
}

} // namespace

void UpdateLivesWidget::Register(const flecs::world& world)
{
    world.system<TextRenderable>("UpdateLivesWidget")
        .with<GameStatePlaying>()
        .singleton()
        .with<LivesWidget>()
        .kind(flecs::PostUpdate)
        .each(Update());
}
