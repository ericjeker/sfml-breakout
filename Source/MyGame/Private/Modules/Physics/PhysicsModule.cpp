// Copyright (c) Eric Jeker 2025.

#include "PhysicsModule.h"

#include "Components/Acceleration.h"
#include "Components/Collider.h"
#include "Components/Friction.h"
#include "Components/Gravity.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "PhysicsConstants.h"

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
    v.velocity *= (1.f - f.friction * it.delta_time());
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

void CollisionSystem(const flecs::iter& it, const size_t idx, Transform& t, Velocity& v, const Collider& c)
{
    ZoneScoped;

    it.world().query<Transform, Velocity, const Collider>().each(
        [&](const flecs::entity other, Transform& t2, Velocity& v2, const Collider& c2)
        {
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
    world.system<const Gravity, Velocity>("GravitySystem").each(GravitySystem);
    world.system<const Friction, Velocity>("FrictionSystem").each(FrictionSystem);
    world.system<Acceleration, Velocity>("AccelerationSystem").each(AccelerationSystem);
    world.system<Transform, const Velocity>("MovementSystem").each(MovementSystem);
    world.system<Transform, Velocity, const Collider>("CollisionSystem").each(CollisionSystem);
}


} // namespace Modules
