// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Physics/PhysicsModule.h"

#include "Core/Modules/Physics/Components/Acceleration.h"
#include "Core/Modules/Physics/Components/CircleCollider.h"
#include "Core/Modules/Physics/Components/Friction.h"
#include "Core/Modules/Physics/Components/Gravity.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Physics/Singletons/GravitySettings.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/PhysicsConstants.h"

#include <numbers>
#include <tracy/Tracy.hpp>

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

void CollisionSystem(const flecs::iter& it, const size_t idx, Transform& t, Velocity& v, const CircleCollider& c)
{
    ZoneScoped;

    it.world().query<Transform, Velocity, const CircleCollider>().each(
        [&](const flecs::entity other, Transform& t2, Velocity& v2, const CircleCollider& c2) {
            if (other == it.entity(idx))
            {
                return;
            }

            // TODO: Implement multiple types of colliders (circle, rectangle, etc)
            const sf::Vector2f difference = t2.position - t.position;
            const float distance = difference.length();
            const float sumOfRadii = c.radius + c2.radius;
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

} // namespace


namespace Modules
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
    world.system<Transform, Velocity, const CircleCollider>("CollisionSystem").each(CollisionSystem);
}


} // namespace Modules
