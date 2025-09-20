// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Render/RenderModule.h"

#include "Core/GameService.h"

#include "Core/Modules/Particles/Components/Particle.h"
#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/RectangleRenderable.h"
#include "Core/Modules/Render/Components/ShaderUniform.h"
#include "Core/Modules/Render/Components/ShaderUniforms.h"
#include "Core/Modules/Render/Components/SpriteRenderable.h"
#include "Core/Modules/Render/Components/TextRenderable.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <map>
#include <tracy/Tracy.hpp>


namespace
{

constexpr bool DEBUG_ORIGIN = false;

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

    if constexpr (!DEBUG_ORIGIN)
    {
        return;
    }

    // Debug Center Point
    sf::CircleShape circleCenter(1.f);
    circleCenter.setFillColor(sf::Color::White);
    circleCenter.setPosition(rect.shape.getPosition() + rect.shape.getOrigin());
    circleCenter.setOrigin({0.5f, 0.5f});

    window.draw(circleCenter);
}

void RenderCircleShape(const CircleRenderable& circle)
{
    const auto drawable = circle.shape;

    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(drawable);

    if constexpr (!DEBUG_ORIGIN)
    {
        return;
    }

    // Debug Center Point
    sf::CircleShape circleCenter(1.f);
    circleCenter.setFillColor(sf::Color::White);
    circleCenter.setPosition(drawable.getPosition());
    circleCenter.setOrigin({0.5f, 0.5f});

    window.draw(circleCenter);
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

/**
 * TODO: this is very inefficient... drawing particles one by one...
 */
void RenderParticle(const Transform& t)
{
    sf::VertexArray vertices{sf::PrimitiveType::Points, 1};
    vertices[0].position = t.position;
    vertices[0].color = sf::Color::White;

    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(vertices);
}

// void RenderShader(const ShaderRenderable& s, const ShaderUniforms& us)
// {
//     if (!sf::Shader::isAvailable())
//     {
//         LOG_ERROR("RenderModule::RenderShader: Shaders are not available");
//         return;
//     }
//
//     for (const auto& [name, value] : us.uniforms)
//     {
//         if (std::empty(name))
//         {
//             LOG_ERROR("RenderModule::RenderShader: Shader uniform name is empty");
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

void RenderAllParticles(const flecs::iter& it)
{
    ZoneScopedN("RenderModule::RenderAllParticles");

    // Collect all particle positions in one pass
    std::vector<sf::Vector2f> particlePositions;
    it.world().each([&](const flecs::entity e, const Transform& t, const Particle& p) {
        // TODO: Frustum culling, test if particle is visible
        particlePositions.push_back(t.position);
    });

    if (particlePositions.empty())
        return;

    // Create one big vertex array for ALL particles
    sf::VertexArray vertices{sf::PrimitiveType::Points, particlePositions.size()};

    for (size_t i = 0; i < particlePositions.size(); ++i)
    {
        vertices[i].position = particlePositions[i];
        vertices[i].color = sf::Color::White;
    }

    // Single draw call for ALL particles!
    auto& window = GameService::Get<sf::RenderWindow>();
    window.draw(vertices);
}

struct RenderableEntry
{
    float zOrder;
    flecs::entity entity;
};

void Render(const flecs::iter& it)
{
    ZoneScopedN("RenderModule::Render");

    // TODO: Reserve vector
    std::vector<RenderableEntry> renderables;

    // TODO: Map by ZOrder and Renderable type so we can draw all of them at the same time on one single layer
    // std::map<float, std::vector<flecs::entity>> renderablesByZOrder;

    const auto world = it.world();
    // Collect all the renderable entities
    world.query_builder<const Transform, const SpriteRenderable, const ZOrder>()
        .with(flecs::Disabled)
        .optional()
        .each([&](const flecs::entity e, const Transform& t, const SpriteRenderable& s, const ZOrder& zOrder) {
            renderables.push_back({zOrder.zOrder, e});
        });
    world.query_builder<const Transform, const CircleRenderable, const ZOrder>()
        .with(flecs::Disabled)
        .optional()
        .each([&](const flecs::entity e, const Transform& t, const CircleRenderable& c, const ZOrder& zOrder) {
            renderables.push_back({zOrder.zOrder, e});
        });
    world.query_builder<const Transform, const RectangleRenderable, const ZOrder>()
        .with(flecs::Disabled)
        .optional()
        .each([&](const flecs::entity e, const Transform& t, const RectangleRenderable& r, const ZOrder& zOrder) {
            renderables.push_back({zOrder.zOrder, e});
        });
    world.query_builder<const Transform, const TextRenderable, const ZOrder>()
        .with(flecs::Disabled)
        .optional()
        .each([&](const flecs::entity e, const Transform& t, const TextRenderable& text, const ZOrder& zOrder) {
            renderables.push_back({zOrder.zOrder, e});
        });
    world.query_builder<const Transform, const Particle, const ZOrder>()
        .with(flecs::Disabled)
        .optional()
        .each([&](const flecs::entity e, const Transform& t, const Particle& p, const ZOrder& zOrder) {
            renderables.push_back({zOrder.zOrder, e});
        });

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
        else if (currentEntity.has<Particle>())
        {
            RenderParticle(currentEntity.get<Transform>());
        }
    }
}

} // namespace

namespace Core::Modules
{

RenderModule::RenderModule(const flecs::world& world)
{
    // --- Declare Components ---
    world.component<CircleRenderable>();
    world.component<RectangleRenderable>();
    // world.component<ShaderRenderable>();
    world.component<ShaderUniform>();
    world.component<ShaderUniforms>();
    world.component<SpriteRenderable>();
    world.component<TextRenderable>();
    world.component<ZOrder>();

    // --- We apply all the Transform to the Renderables ---
    world.system<const Transform, CircleRenderable>("RenderModule::ApplyTransformToCircle").kind(flecs::PreStore).each(ApplyTransformToCircle);
    world.system<const Transform, RectangleRenderable>("RenderModule::ApplyTransformToRectangle")
        .kind(flecs::PreStore)
        .each(ApplyTransformToRectangle);
    world.system<const Transform, SpriteRenderable>("RenderModule::ApplyTransformToSprite").kind(flecs::PreStore).each(ApplyTransformToSprite);

    // --- Render all the Renderable Components ---
    world.system("RenderModule::Render").kind(flecs::OnStore).run(Render);
    world.system("RenderModule::RenderParticles").kind(flecs::OnStore).run(RenderAllParticles);
}

} // namespace Modules
