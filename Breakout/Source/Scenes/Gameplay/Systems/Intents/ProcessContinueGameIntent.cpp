// Copyright (c) Eric Jeker 2025.

#include "ProcessContinueGameIntent.h"

#include "Scenes/Gameplay/Components/Intents/ContinueGameIntent.h"
#include "Scenes/Gameplay/Components/Paddle.h"
#include "Scenes/Gameplay/Entities/BallEntity.h"

#include "Core/Configuration.h"
#include "Core/Modules/Render/Components/Transform.h"

#include <functional>

namespace
{

std::function<void(flecs::entity, ContinueGameIntent)> Update(const flecs::entity& rootEntity)
{
    return [&](const flecs::entity& e, const ContinueGameIntent& i) {
        // Create a ball and place it on the paddle
        BallEntity::Create(e.world(), rootEntity, 0.f);

        // Center the paddle again on the screen
        e.world().query<const Paddle>().each([](const flecs::entity& paddle, const Paddle& p) {
            paddle.set<Transform>({{Configuration::RESOLUTION.x / 2, Configuration::RESOLUTION.y - 100.f}});
        });
    };
}

} // namespace

void ProcessContinueGameIntent::Register(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<const ContinueGameIntent>("ProcessContinueGameIntent")
        .kind(flecs::PreUpdate)
        .each(Update(rootEntity))
        .child_of(rootEntity);
}
