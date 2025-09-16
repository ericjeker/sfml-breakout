// Copyright (c) Eric Jeker 2025.

#include "ConstrainPaddleToScreenSystem.h"

#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Scenes/Gameplay/Components/Paddle.h"

#include "Core/Configuration.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"

namespace
{

void Update(Transform& t, const Size& s, const Paddle& p)
{
    const float halfSize = s.size.x / 2;

    if (t.position.x - halfSize < 0.f)
    {
        t.position.x = halfSize;
    }
    else if (t.position.x + halfSize > Configuration::RESOLUTION.x)
    {
        t.position.x = Configuration::RESOLUTION.x - halfSize;
    }
}

} // namespace

void ConstrainPaddleToScreenSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<Transform, const Size, const Paddle>("ConstrainPaddleToScreenSystem")
        .with<GameStatePlaying>().singleton()
        .each(Update)
        .child_of(rootEntity);
}
