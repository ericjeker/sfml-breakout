// Copyright (c) Eric Jeker 2025.

#include "Components/CircleShape.h"
#include "Components/RectangleShape.h"
#include "Components/TextRenderable.h"
#include "Managers/GameService.h"
#include "RenderModule.h"

#include <SFML/Graphics/RenderWindow.hpp>


namespace
{

void RenderRectangleShape(const RectangleShape& bg)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*bg.shape);
}

void RenderCircleShape(const CircleShape& ball)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*ball.shape);
}

void RenderText(const TextRenderable& text)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*text.text);
}

}

namespace Modules
{

RenderModule::RenderModule(flecs::world& world)
{
    world.component<RectangleShape>();
    world.component<CircleShape>();
    world.component<TextRenderable>();

    world.system<const RectangleShape>("RectangleShape").kind(flecs::OnStore).each(RenderRectangleShape);
    world.system<const CircleShape>("CircleShape").kind(flecs::OnStore).each(RenderCircleShape);
    world.system<const TextRenderable>("TextRenderable").kind(flecs::OnStore).each(RenderText);
}

}
