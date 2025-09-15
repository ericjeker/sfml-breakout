// Copyright (c) Eric Jeker 2025.

#include "CheckAllBlocksDestroyedSystem.h"

#include "GameStates/Gameplay/Components/CurrentLevel.h"
#include "GameStates/Gameplay/Components/MaxLevel.h"
#include "Scenes/Gameplay/Components/Block.h"
#include "Scenes/Gameplay/Components/GameWonIntent.h"
#include "Scenes/Gameplay/Components/Indestructible.h"
#include "Scenes/Gameplay/Components/NextLevelIntent.h"

#include "Core/Modules/Input/Components/Command.h"
#include "Core/Modules/Lifetime/Components/LifetimeOneFrame.h"
#include "Core/Scenes/Tags/ScenePaused.h"
#include "Core/Singletons/FrameCount.h"
#include "Core/Utils/Logger.h"

namespace
{

auto Update(const flecs::entity& rootEntity)
{
    return [rootEntity](const flecs::iter& it) {
        if (rootEntity.has<ScenePaused>())
        {
            return;
        }

        const auto& world = it.world();
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
        world.entity().add<LifetimeOneFrame>().add<Command>().add<GameWonIntent>();
        }
    };
}

} // namespace

void CheckAllBlocksDestroyedSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    // Unmatched task
    world.system("CheckAllBlocksDestroyedSystem")
        .kind(flecs::PostUpdate)
        .write<NextLevelIntent>()
        .run(Update(rootEntity))
        .child_of(rootEntity);
}
