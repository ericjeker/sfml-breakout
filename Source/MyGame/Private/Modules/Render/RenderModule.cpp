// Copyright (c) Eric Jeker 2025.

#include "RenderModule.h"

#include "Components/CircleRenderable.h"
#include "Components/RectangleRenderable.h"
#include "Components/ShaderUniform.h"
#include "Components/ShaderUniforms.h"
#include "Components/SpriteRenderable.h"
#include "Components/TextRenderable.h"
#include "Components/Transform.h"
#include "Managers/GameService.h"

#include <SFML/Graphics/RenderWindow.hpp>


namespace
{

void ApplyTransform(const Transform& t, CircleRenderable& c)
{
    c.shape.setPosition(t.position);
    c.shape.setScale(t.scale);
    c.shape.setRotation(sf::degrees(t.rotation));
}

void RenderRectangleShape(const RectangleRenderable& rect)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(rect.shape);
}

void RenderCircleShape(const CircleRenderable& ball)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(ball.shape);
}

void RenderText(const TextRenderable& text)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*text.text);
}

void RenderSprite(const SpriteRenderable& s)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(s.sprite);
}

// void RenderShader(const ShaderRenderable& s, const ShaderUniforms& us)
// {
//     if (!sf::Shader::isAvailable())
//     {
//         LOG_ERROR("(RenderModule::RenderShader): Shaders are not available");
//         return;
//     }
//
//     for (const auto& [name, value] : us.uniforms)
//     {
//         if (std::empty(name))
//         {
//             LOG_ERROR("(RenderModule::RenderShader): Shader uniform name is empty");
//             continue;
//         }
//
//         // std::visit([&](const auto& v) { setter.setUniform(name, v); }, value);
//     }
//
//     // Create render states with our shader
//     sf::RenderStates states;
//     states.shader = &s.shader;
//
//     auto& window = GameService::Get<sf::RenderWindow>();
//     window.draw(*s.target, states);
// }

} // namespace

namespace Modules
{

RenderModule::RenderModule(const flecs::world& world)
{
    world.component<RectangleRenderable>();
    world.component<CircleRenderable>();
    world.component<TextRenderable>();
    world.component<SpriteRenderable>();
    // world.component<ShaderRenderable>();
    world.component<ShaderUniform>();
    world.component<ShaderUniforms>();

    world.system<const Transform, CircleRenderable>("ApplyTransform").kind(flecs::PreStore).each(ApplyTransform);
    world.system<const RectangleRenderable>("RectangleRenderable").kind(flecs::OnStore).each(RenderRectangleShape);
    world.system<const CircleRenderable>("CircleRenderable").kind(flecs::OnStore).each(RenderCircleShape);
    world.system<const TextRenderable>("TextRenderable").kind(flecs::OnStore).each(RenderText);
    world.system<const SpriteRenderable>("SpriteRenderable").kind(flecs::OnStore).each(RenderSprite);
    // world.system<const ShaderRenderable>("ShaderRenderable").kind(flecs::OnStore).each(RenderShader);
}

} // namespace Modules
