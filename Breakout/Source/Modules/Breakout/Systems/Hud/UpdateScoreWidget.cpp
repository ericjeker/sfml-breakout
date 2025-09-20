// Copyright (c) Eric Jeker. All Rights Reserved.

#include "UpdateScoreWidget.h"

#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/Score.h"
#include "Modules/Breakout/Tags/ScoreWidget.h"

#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/TextRenderable.h"

namespace
{

auto Update()
{
    return [](const flecs::entity& e, const TextRenderable& t) {
        const auto& [score, blocksDestroyed, timeElapsed] = e.world().get<Score>();
        const auto& [origin] = e.get<Origin>();

        t.text->setString("SCORE " + std::to_string(score));

        // Update the origin based on the text bounds
        const sf::FloatRect textBounds = t.text->getLocalBounds();
        t.text->setOrigin(
            {textBounds.size.x * origin.x + textBounds.position.x, textBounds.size.y * origin.y + textBounds.position.y}
        );
    };
}

} // namespace

void UpdateScoreWidget::Register(const flecs::world& world)
{
    world.system<TextRenderable>("UpdateScoreWidget")
        .with<GameStatePlaying>()
        .singleton()
        .with<ScoreWidget>()
        .kind(flecs::PostUpdate)
        .each(Update());
}
