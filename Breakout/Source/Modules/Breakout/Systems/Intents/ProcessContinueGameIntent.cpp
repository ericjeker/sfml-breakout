// Copyright (c) Eric Jeker 2025.

#include "ProcessContinueGameIntent.h"

#include "Modules/Breakout/Components/Intents/ContinueGameIntent.h"
#include "Modules/Breakout/Components/Paddle.h"
#include "Scenes/Gameplay/Entities/BallEntity.h"

#include "Core/Configuration.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Scene/Components/ActiveScene.h"

#include <functional>

namespace
{

std::function<void(flecs::entity, ContinueGameIntent)> Update()
{
    return [](const flecs::entity& e, const ContinueGameIntent& i) {
        // Get the ActiveScene
        const auto& [rootEntity] = e.world().get<ActiveScene>();

        // Create a ball and place it on the paddle
        BallEntity::Create(e.world(), rootEntity, 0.f);

        // Center the paddle again on the screen
        e.world().query<const Paddle>().each([](const flecs::entity& paddle, const Paddle& p) {
            paddle.set<Transform>({.position = {Configuration::RESOLUTION.x / 2, Configuration::RESOLUTION.y - 100.f}});
        });
    };
}

} // namespace

void ProcessContinueGameIntent::Register(const flecs::world& world)
{
    world.system<const ContinueGameIntent>("ProcessContinueGameIntent").kind(flecs::PreUpdate).each(Update());
}
