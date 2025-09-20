// Copyright (c) Eric Jeker 2025.

#include "OutOfBoundsSystem.h"

#include "../../../Modules/Breakout/Singletons/Lives.h"
#include "../../../Modules/Breakout/Singletons/Multiplier.h"
#include "../Components/Intents/ContinueGameIntent.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/Intents/GameOverIntent.h"

#include "Core/Configuration.h"
#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Utils/Logger.h"

namespace
{

void Update(const flecs::entity ball, const Transform& t, const Ball& b)
{
    if (t.position.y > Configuration::RESOLUTION.y)
    {
        LOG_DEBUG("GameplayScene::ProcessOutOfBounds -> Destroy ball");
        ball.destruct();

        // Lose one life
        LOG_DEBUG("GameplayScene::ProcessOutOfBounds -> LoseOneLife");
        ball.world().get_mut<Lives>().lives -= 1;
        if (ball.world().get_mut<Lives>().lives <= 0)
        {
            // Game over
            LOG_DEBUG("GameplayScene::ProcessOutOfBounds -> Add GameOverIntent");
            ball.world().entity().add<LifetimeOneFrame>().add<Command>().add<GameOverIntent>();
        }
        else
        {
            // Continue playing...
            ball.world().entity().add<LifetimeOneFrame>().add<Command>().add<ContinueGameIntent>();
        }

        ball.world().get_mut<Multiplier>().multiplier = 1;
    }
}

} // namespace


void OutOfBoundsSystem::Register(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<Transform, const Ball>("OutOfBoundsSystem")
        .write<ContinueGameIntent>()
        .write<GameOverIntent>()
        .with<GameStatePlaying>().singleton()
        .each(Update)
        .child_of(rootEntity);
}
