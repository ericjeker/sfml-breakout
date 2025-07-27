// Copyright (c) Eric Jeker 2025.

#include "RenderModule.h"

#include "Components/BackgroundRenderable.h"
#include "Components/BallRenderable.h"
#include "Components/TextRenderable.h"
#include "Managers/GameService.h"

#include <SFML/Graphics/RenderWindow.hpp>


namespace
{

void RenderBackground(const BackgroundRenderable& bg)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*bg.shape);
}

void RenderBalls(const BallRenderable& ball)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*ball.shape);
}

void RenderText(const TextRenderable& text)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*text.text);
}

} // namespace

namespace RenderModule
{

RenderModule::RenderModule(flecs::world& world)
{
    world.component<BackgroundRenderable>();
    world.component<BallRenderable>();
    world.component<TextRenderable>();

    world.system<const BackgroundRenderable>("BackgroundRenderable").kind(flecs::OnStore).each(RenderBackground);
    world.system<const BallRenderable>("BallRenderable").kind(flecs::OnStore).each(RenderBalls);
    world.system<const TextRenderable>("TextRenderable").kind(flecs::OnStore).each(RenderText);
}

} // namespace RenderModule
