// Copyright (c) Eric Jeker 2025.

#include "OutOfBoundsSystem.h"

#include "GameStates/Gameplay/Components/Lives.h"
#include "GameStates/Gameplay/Components/Multiplier.h"
#include "Scenes/Gameplay/Components/Ball.h"
#include "Scenes/Gameplay/Components/ContinueGameIntent.h"
#include "Scenes/Gameplay/Components/GameOverIntent.h"

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


void OutOfBoundsSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<Transform, const Ball>("OutOfBoundsSystem")
        .write<ContinueGameIntent>()
        .write<GameOverIntent>()
        .each(Update)
        .child_of(rootEntity);
}
