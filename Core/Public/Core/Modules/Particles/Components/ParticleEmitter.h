// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Modules/Render/Components/Transform.h"

#include <flecs.h>
#include <functional>


struct ParticleEmitter;
using ParticleGenerator = std::function<flecs::entity(flecs::world, const ParticleEmitter&, const Transform&)>;

struct ParticleEmitter
{
    // Tuning
    float ratePerSecond{100.f};
    int maxParticles{1000};

    // Particle settings
    float minLifetime{0.5f};
    float maxLifetime{1.2f};
    float minVelocity{20.f};
    float maxVelocity{80.f};

    // The emitter itself has a Transform rotation
    float minAngleDeg{-10.f};
    float maxAngleDeg{10.f};

    // State
    bool enabled{true};
    bool loop{false};

    // Custom particle generator
    ParticleGenerator generator;

    // Runtime
    float spawnAccumulator{0.f};
};

