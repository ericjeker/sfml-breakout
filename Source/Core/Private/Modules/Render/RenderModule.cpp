// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Render/RenderModule.h"

#include "Core/Managers/GameService.h"
#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/RectangleRenderable.h"
#include "Core/Modules/Render/Components/ShaderUniform.h"
#include "Core/Modules/Render/Components/ShaderUniforms.h"
#include "Core/Modules/Render/Components/SpriteRenderable.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <tracy/Tracy.hpp>


namespace
{

void ApplyTransformToCircle(const Transform& t, CircleRenderable& c)
{
    c.shape.setPosition(t.position);
    c.shape.setScale(t.scale);
    c.shape.setRotation(sf::degrees(t.rotation));
}

void ApplyTransformToRectangle(const Transform& t, RectangleRenderable& r)
{
    r.shape.setPosition(t.position);
    r.shape.setScale(t.scale);
    r.shape.setRotation(sf::degrees(t.rotation));
}

void ApplyTransformToSprite(const Transform& t, SpriteRenderable& s)
{
    s.sprite->setPosition(t.position);
    s.sprite->setScale(t.scale);
    s.sprite->setRotation(sf::degrees(t.rotation));
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
    if (!text.text)
    {
        return;
    }

    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*text.text);
}

void RenderSprite(const SpriteRenderable& s)
{
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(*s.sprite);
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

struct RenderableEntry
{
    float zOrder;
    flecs::entity entity;
};

void Render(const flecs::iter& it)
{
    ZoneScopedN("RenderModule::Render");

    std::vector<RenderableEntry> renderables;

    // Collect all the renderable entities
    it.world().each([&](const flecs::entity e, const Transform& t, const SpriteRenderable& s, const ZOrder& zOrder)
                    { renderables.push_back({zOrder.zOrder, e}); });
    it.world().each([&](const flecs::entity e, const Transform& t, const CircleRenderable& c, const ZOrder& zOrder)
                    { renderables.push_back({zOrder.zOrder, e}); });
    it.world().each([&](const flecs::entity e, const Transform& t, const RectangleRenderable& r, const ZOrder& zOrder)
                    { renderables.push_back({zOrder.zOrder, e}); });
    it.world().each([&](const flecs::entity e, const Transform& t, const TextRenderable& text, const ZOrder& zOrder)
                    { renderables.push_back({zOrder.zOrder, e}); });

    // Sort the collected renderable entities by their ZOrder value
    std::ranges::sort(renderables, [](const RenderableEntry& a, const RenderableEntry& b) { return a.zOrder < b.zOrder; });

    // Iterate through the sorted queue and draw each entity
    for (const auto& [zOrder, entity] : renderables)
    {
        if (const flecs::entity currentEntity = entity; currentEntity.has<SpriteRenderable>())
        {
            RenderSprite(currentEntity.get<SpriteRenderable>());
        }
        else if (currentEntity.has<CircleRenderable>())
        {
            RenderCircleShape(currentEntity.get<CircleRenderable>());
        }
        else if (currentEntity.has<RectangleRenderable>())
        {
            RenderRectangleShape(currentEntity.get<RectangleRenderable>());
        }
        else if (currentEntity.has<TextRenderable>())
        {
            RenderText(currentEntity.get<TextRenderable>());
        }
    }
}

} // namespace

namespace Modules
{

RenderModule::RenderModule(const flecs::world& world)
{
    world.component<ZOrder>();
    world.component<RectangleRenderable>();
    world.component<CircleRenderable>();
    world.component<TextRenderable>();
    world.component<SpriteRenderable>();
    // world.component<ShaderRenderable>();
    world.component<ShaderUniform>();
    world.component<ShaderUniforms>();

    world.system<const Transform, CircleRenderable>("ApplyTransformToCircle").kind(flecs::PreStore).each(ApplyTransformToCircle);
    world.system<const Transform, RectangleRenderable>("ApplyTransformToRectangle")
        .kind(flecs::PreStore)
        .each(ApplyTransformToRectangle);
    world.system<const Transform, SpriteRenderable>("ApplyTransformToSprite").kind(flecs::PreStore).each(ApplyTransformToSprite);

    // Render all the Renderable Components
    world.system().kind(flecs::OnStore).run(Render);
}

} // namespace Modules
