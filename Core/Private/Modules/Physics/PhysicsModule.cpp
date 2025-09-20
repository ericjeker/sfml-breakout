// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Physics/PhysicsModule.h"

#include "Core/GameService.h"

#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/CollisionInfo.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Physics/Singletons/GravitySettings.h"
#include "Core/Modules/Render/Components/CircleRenderable.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/RectangleRenderable.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"
#include "Core/PhysicsConstants.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <numbers>
#include <tracy/Tracy.hpp>

namespace sf
{
class RenderWindow;
}
namespace
{

void GravitySystem(const flecs::iter& it, size_t, const Gravity& g, Velocity& v)
{
    // Both Gravity and Acceleration are expressed in cm/s
    v.velocity.x += g.gravity.x * PhysicsConstants::PIXELS_PER_CENTIMETER * it.delta_time();
    v.velocity.y += g.gravity.y * PhysicsConstants::PIXELS_PER_CENTIMETER * it.delta_time();
}

void FrictionSystem(const flecs::iter& it, size_t, const Friction& f, Velocity& v)
{
    const float frictionFactory = std::max(0.f, 1.f - f.friction * it.delta_time());
    v.velocity *= frictionFactory;
}

void AccelerationSystem(const flecs::iter& it, size_t, Acceleration& a, Velocity& v)
{
    // Both Gravity and Acceleration are expressed in cm/s
    v.velocity += a.acceleration * PhysicsConstants::PIXELS_PER_CENTIMETER * it.delta_time();
    a.acceleration = {0.f, 0.f};
}

void MovementSystem(const flecs::iter& it, size_t, Transform& t, const Velocity& v)
{
    t.position += v.velocity * it.delta_time();
}

void CircleCollisionSystem(const flecs::iter& it, const size_t idx, Transform& t, Velocity& v, const Radius& r, const ColliderShape& c)
{
    ZoneScoped;

    it.world().query<Transform, Velocity, const Radius, const ColliderShape>().each(
        [&](const flecs::entity other, Transform& t2, Velocity& v2, const Radius& r2, const ColliderShape& c2) {
            if (other == it.entity(idx))
            {
                return;
            }

            // TODO: Implement multiple types of colliders (circle, rectangle, etc)
            const sf::Vector2f difference = t2.position - t.position;
            const float distance = difference.length();
            const float sumOfRadii = r.radius + r2.radius;
            if (distance > sumOfRadii || distance == 0.f)
            {
                // No collision :(
                return;
            }

            const sf::Vector2f collisionNormal = difference.normalized();
            const sf::Vector2f relativeVelocity = v2.velocity - v.velocity;
            const float velocityNormal = relativeVelocity.dot(collisionNormal);

            if (velocityNormal > 0.f)
            {
                return;
            }

            v.velocity += collisionNormal * velocityNormal;
            v2.velocity -= collisionNormal * velocityNormal;

            // Move the balls outside each others
            const float overlap = sumOfRadii - distance;
            t.position -= collisionNormal * overlap * 0.5f;
            t2.position += collisionNormal * overlap * 0.5f;
        }
    );
}

void AddDebugCircleCollider(const flecs::entity& e, const Transform& t, const Origin& o, const Radius& r, const ColliderShape& c)
{
    sf::CircleShape circle;
    circle.setPosition(t.position);
    circle.setRadius(r.radius);
    circle.setOrigin({2 * r.radius * o.origin.x, 2 * r.radius * o.origin.y});
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(1.f);
    circle.setOutlineColor(sf::Color::Magenta);

    e.set<CircleRenderable>({circle}).set<ZOrder>({ZOrderLayer::Debug});
}

void AddDebugRectCollider(const flecs::entity& e, const Transform& t, const Origin& o, const Size& s, const ColliderShape& c)
{
    sf::RectangleShape rect;
    rect.setPosition(t.position);
    rect.setSize(s.size);
    rect.setOrigin({s.size.x * o.origin.x, s.size.y * o.origin.y});
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Magenta);

    e.set<RectangleRenderable>({rect}).set<ZOrder>({ZOrderLayer::Debug});
}

void AddDebugCollisionInfo(const flecs::entity& e, const CollisionInfo& c)
{
    constexpr float SIZE = 5.f;

    sf::RectangleShape rect;
    rect.setPosition(c.contactPoint);
    rect.setSize({SIZE, SIZE});
    rect.setOrigin({SIZE * 0.5f, SIZE * 0.5f});
    rect.setFillColor(sf::Color::White);

    e.set<Transform>({.position = c.contactPoint}).set<RectangleRenderable>({rect}).set<ZOrder>({ZOrderLayer::Debug});
}

} // namespace


namespace Core::Modules
{

PhysicsModule::PhysicsModule(const flecs::world& world)
{
    world.component<Velocity>().add(flecs::CanToggle);

    world.set<GravitySettings>(
        {.gravity = PhysicsConstants::NO_GRAVITY, .pixelsPerCentimeter = PhysicsConstants::PIXELS_PER_CENTIMETER}
    );

    world.system<const Gravity, Velocity>("GravitySystem").each(GravitySystem);
    world.system<const Friction, Velocity>("FrictionSystem").each(FrictionSystem);
    world.system<Acceleration, Velocity>("AccelerationSystem").each(AccelerationSystem);
    world.system<Transform, const Velocity>("MovementSystem").each(MovementSystem);
    world.system<Transform, Velocity, const Radius, const ColliderShape>("CircleCollisionSystem").each(CircleCollisionSystem);

    // Debug rendering
    //world.system<const Transform, const Origin, const Radius, const ColliderShape>("AddDebugCircleCollider").each(AddDebugCircleCollider);
    //world.system<const Transform, const Origin, const Size, const ColliderShape>("AddDebugRectCollider").each(AddDebugRectCollider);
    world.system<const CollisionInfo>("AddDebugCollisionInfo").each(AddDebugCollisionInfo);
}


} // namespace Modules
