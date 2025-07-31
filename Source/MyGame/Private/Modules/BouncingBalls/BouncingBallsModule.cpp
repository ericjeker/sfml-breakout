// Copyright (c) Eric Jeker 2025.

#include "BouncingBallsModule.h"

#include "Components/Transform.h"
#include "Configuration.h"
#include "Modules/Physics/Components/Velocity.h"


namespace
{


void ProcessScreenBounce(Transform& t, Velocity& v)
{
    if (t.position.x < 0.f)
    {
        v.velocity.x *= -1.f;
        t.position.x = 0.f;
    }
    else if (t.position.x > Configuration::WINDOW_SIZE.x)
    {
        v.velocity.x *= -1.f;
        t.position.x = Configuration::WINDOW_SIZE.x;
    }

    if (t.position.y < 0.f)
    {
        v.velocity.y *= -1.f;
        t.position.y = 0.f;
    }
    else if (t.position.y > Configuration::WINDOW_SIZE.y)
    {
        v.velocity.y *= -1.f;
        t.position.y = Configuration::WINDOW_SIZE.y;
    }
}


// void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, CircleRenderable& ball)
// {
//     // Physics system
//     if (auto [gravity, pixelsPerCentimeter, enabled] = it.world().get<GravitySettings>(); enabled)
//     {
//         v.velocity.x += gravity.x * pixelsPerCentimeter * it.delta_time();
//         v.velocity.y += gravity.y * pixelsPerCentimeter * it.delta_time();
//     }
//
//     v.velocity *= p.damping;
//     t.position += v.velocity * it.delta_time();
//
//     ball.shape.setPosition(t.position);
// }

}


namespace Modules
{

BouncingBallsModule::BouncingBallsModule(const flecs::world& world)
{
    world.system<Transform, Velocity>().kind(flecs::OnStore).each(ProcessScreenBounce);
}

} // namespace Modules
