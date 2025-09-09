// Copyright (c) Eric Jeker 2025.

#include "ScreenBounceSystem.h"

#include "Scenes/Gameplay/Components/Ball.h"

#include "Core/Configuration.h"
#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Transform.h"

namespace
{

void ProcessScreenBounce(Transform& t, Velocity& v, const ColliderShape& c, const Radius& r, const Ball& b)
{
    const float radius = r.radius;

    if (t.position.x - radius < 0.f)
    {
        v.velocity.x *= -1.f;
        t.position.x = radius;
    }
    else if (t.position.x + radius > Configuration::RESOLUTION.x)
    {
        v.velocity.x *= -1.f;
        t.position.x = Configuration::RESOLUTION.x - radius;
    }

    if (t.position.y - radius < 0.f)
    {
        v.velocity.y *= -1.f;
        t.position.y = radius;
    }
}

} // namespace

void ScreenBounceSystem::Initialize(const flecs::world& world, const flecs::entity& rootEntity)
{
    world.system<Transform, Velocity, const ColliderShape, const Radius, const Ball>("ScreenBounceSystem")
    .each(ProcessScreenBounce)
    .child_of(rootEntity);
}
