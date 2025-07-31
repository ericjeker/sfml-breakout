// Copyright (c) Eric Jeker 2025.

#include "PhysicsModule.h"

#include "Components/Acceleration.h"
#include "Components/Friction.h"
#include "Components/Gravity.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"

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

} // namespace


namespace Modules
{

PhysicsModule::PhysicsModule(const flecs::world& world)
{
    world.system<const Gravity, Velocity>("GravitySystem").each(GravitySystem);
    world.system<const Friction, Velocity>("FrictionSystem").each(FrictionSystem);
    world.system<Acceleration, Velocity>("AccelerationSystem").each(AccelerationSystem);
    world.system<Transform, const Velocity>("MovementSystem").each(MovementSystem);
}


} // namespace Modules
