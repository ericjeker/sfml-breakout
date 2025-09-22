// Copyright (c) Eric Jeker 2025.

#include "Core/Modules/Particles/ParticlesModule.h"

#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Particles/Components/Particle.h"
#include "Core/Modules/Particles/Components/ParticleEmitter.h"
#include "Core/Modules/Particles/Prefabs/Particle.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Modules/Render/Components/ZOrder.h"
#include "Core/Utils/Random.h"

namespace
{

ParticleGenerator CreateDefaultParticleGenerator()
{
    return [](flecs::world world, const ParticleEmitter& emitter, const Transform& emitterTransform) -> flecs::entity
    {
        const auto velocity = Random::UniformFloat(emitter.minVelocity, emitter.maxVelocity);
        const sf::Vector2f v = {Random::UniformFloat(-velocity, velocity), Random::UniformFloat(-velocity, velocity)};
        return world.entity()
            .is_a<Prefabs::Particle>()
            .set<Velocity>({v})
            .set<ZOrder>({1000.f})
            .set<Transform>({.position = emitterTransform.position})
            .set<Lifetime>({Random::UniformFloat(emitter.minLifetime, emitter.maxLifetime)});
    };
}

void EmitParticles(const flecs::iter& it, size_t idx, ParticleEmitter& emitter, const Transform& transform)
{
    if (!emitter.enabled)
    {
        return;
    }

    const float dt = it.world().delta_time();

    // Determine how many new particles to spawn
    emitter.spawnAccumulator += emitter.ratePerSecond * dt;
    const int toSpawn = static_cast<int>(emitter.spawnAccumulator);
    if (toSpawn <= 0)
    {
        return;
    }
    emitter.spawnAccumulator -= static_cast<float>(toSpawn);

    // Enforce capacity
    int countParticles = 0;
    it.entity(idx).children(
        [&](const flecs::entity e)
        {
            if (e.has<Particle>())
            {
                countParticles++;
            }
        }
    );

    if (countParticles > emitter.maxParticles)
    {
        return;
    }

    // Set default generator if none provided
    if (!emitter.generator)
    {
        emitter.generator = CreateDefaultParticleGenerator();
    }

    for (int i = 0; i < toSpawn; i++)
    {
        emitter.generator(it.world(), emitter, transform);
    }
}

} // namespace

namespace Core::Modules
{

ParticlesModule::ParticlesModule(const flecs::world& world)
{
    world.component<ParticleEmitter>();
    world.component<Particle>();

    world.prefab<Prefabs::Particle>().add<Velocity>().add<Transform>().add<Particle>().add<ZOrder>().add<Lifetime>();

    world.system<ParticleEmitter, const Transform>("ParticleEmitterSystem").kind(flecs::OnUpdate).each(EmitParticles);
}

} // namespace Modules
