// Copyright (c) Eric Jeker 2025.


#include "CheckAllBlocksDestroyedSystem.h"

#include "Modules/Breakout/Components/Block.h"
#include "Modules/Breakout/Components/Indestructible.h"
#include "Modules/Breakout/Components/Intents/GameWonIntent.h"
#include "Modules/Breakout/Components/Intents/NextLevelIntent.h"
#include "Modules/Breakout/Components/Intents/TransitionGameStateIntent.h"
#include "Modules/Breakout/Singletons/CurrentLevel.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/MaxLevel.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Modules/Window/Singletons/FrameCount.h"
#include "Core/Utils/Logger.h"

namespace
{

auto Update()
{
    return [](const flecs::iter& it) {
        const auto& world = it.world();
        if (!world.has<GameStatePlaying>())
        {
            return;
        }

        if (world.query_builder<const Block>().without<Indestructible>().build().count() > 0)
        {
            return;
        }

        const int frameCount = world.get<FrameCount>().frameCount;
        LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> All blocks destroyed in frame {}", frameCount);

        const int currentLevel = world.get<CurrentLevel>().level;
        if (const int& maxLevel = world.get<MaxLevel>().level; currentLevel < maxLevel)
        {
            LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> Add NextLevelIntent");
            world.entity().add<LifetimeOneFrame>().add<Command>().add<NextLevelIntent>();
        }
        else
        {
            LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> Add GameWonIntent");
            world.entity().set<TransitionGameStateIntent>({GameTransitions::GameWon});
        }
    };
}

} // namespace

void CheckAllBlocksDestroyedSystem::Register(const flecs::world& world)
{
    // Unmatched task
    world.system("CheckAllBlocksDestroyedSystem")
        .kind(flecs::PostUpdate)
        .write<NextLevelIntent>()
        .write<GameWonIntent>()
        .run(Update());
}
