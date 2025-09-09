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
#include "Core/Utils/Logger.h"

namespace
{

void Update(const flecs::iter& it)
{
    if (it.world().query_builder<const Block>().without<Indestructible>().build().count() > 0)
    {
        return;
    }

    const int currentLevel = it.world().get<CurrentLevel>().level;
    if (const int& maxLevel = it.world().get<MaxLevel>().level; currentLevel < maxLevel)
    {
        LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> Add NextLevelIntent");
        it.world().entity().add<LifetimeOneFrame>().add<Command>().add<NextLevelIntent>();
    }
    else
    {
        LOG_DEBUG("GameplayScene::CheckIfAllBlocksDestroyed -> Add GameWonIntent");
        it.world().entity().add<LifetimeOneFrame>().add<Command>().add<GameWonIntent>();
    }
}

} // namespace

void CheckAllBlocksDestroyedSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system("CheckAllBlocksDestroyedSystem").run(Update).child_of(rootEntity);
}
