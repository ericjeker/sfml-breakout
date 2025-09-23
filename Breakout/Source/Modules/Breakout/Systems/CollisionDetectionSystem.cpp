// Copyright (c) Eric Jeker 2025.


#include "CollisionDetectionSystem.h"

#include "Modules/Breakout/Components/Ball.h"
#include "Modules/Breakout/Components/Health.h"
#include "Modules/Breakout/Components/Indestructible.h"
#include "Modules/Breakout/Components/Paddle.h"
#include "Modules/Breakout/Singletons/GameStatePlaying.h"
#include "Modules/Breakout/Singletons/Multiplier.h"
#include "Modules/Breakout/Singletons/Score.h"

#include "Core/Modules/Camera/Components/CameraShakeIntent.h"
#include "Core/Modules/Lifetime/Components/Lifetime.h"
#include "Core/Modules/Particles/Components/ParticleEmitter.h"
#include "Core/Modules/Physics/Components/ColliderShape.h"
#include "Core/Modules/Physics/Components/CollisionInfo.h"
#include "Core/Modules/Physics/Components/Velocity.h"
#include "Core/Modules/Physics/Utilities/Collision.h"
#include "Core/Modules/Render/Components/Origin.h"
#include "Core/Modules/Render/Components/Radius.h"
#include "Core/Modules/Render/Components/Size.h"
#include "Core/Modules/Render/Components/Transform.h"
#include "Core/Utils/Vector.h"

#include <tracy/Tracy.hpp>

#include <cmath>

namespace
{

sf::Vector2f CalculateReflection(const sf::Vector2f& velocity, const sf::Vector2f& normal)
{
    // Reflect velocity along collision normal
    sf::Vector2f reflected = velocity - 2.f * Vector::Dot(velocity, normal) * normal;

    // Preserve the current speed
    const float currentSpeed = velocity.length();

    // Define the minimum vertical velocity required
    constexpr float MIN_VERTICAL_VELOCITY = 150.f;

    if (std::abs(reflected.y) < MIN_VERTICAL_VELOCITY)
    {
        reflected.y = std::copysign(MIN_VERTICAL_VELOCITY, reflected.y);
    }

    // Scale the velocity to maintain the same speed
    if (reflected.length() > 0.f)
    {
        reflected *= currentSpeed / reflected.length();
    }
    else
    {
        // Fallback in case of zero velocity
        reflected = sf::Vector2f{0.f, MIN_VERTICAL_VELOCITY};
    }

    return reflected;
}

void ProcessCollisionDetection(const flecs::entity& blockEntity, const Transform& transform, const Size& size, const Origin& origin, const ColliderShape&)
{
    ZoneScopedN("CollisionDetectionSystem");

    // Calculate actual collision bounds using transform, size, and origin
    const sf::Vector2f actualOrigin = size.size.componentWiseMul(origin.origin);
    const sf::FloatRect colliderBounds(
        {
            transform.position.x - actualOrigin.x,
            transform.position.y - actualOrigin.y,
        },
        {size.size.x, size.size.y}
    );

    // Check if this is a paddle collision
    const bool isPaddle = blockEntity.has<Paddle>();

    // We query for all the balls
    blockEntity.world().query<Transform, Velocity, const Radius, const ColliderShape, const Ball>().each(
        [blockEntity,
         colliderBounds,
         isPaddle](const flecs::entity&, Transform& ballTransform, Velocity& ballVelocity, const Radius& ballRadius, const ColliderShape&, const Ball&) {
            const auto [hasCollision, contactPoint, normal, penetrationDepth] = Core::
                CheckAABBCircleCollision(colliderBounds, ballTransform.position, ballRadius.radius);

            if (!hasCollision)
            {
                return;
            }

            // Move circle out of penetration
            ballTransform.position = ballTransform.position + normal * penetrationDepth;

            const auto& world = blockEntity.world();

            // Emit particles at the contact point
            world.entity()
                .set<ParticleEmitter>({.ratePerSecond = 50.f})
                .set<Lifetime>({0.1})
                .set<Transform>({.position = contactPoint});

            if (isPaddle)
            {
                // Calculate where on the paddle the ball hit relative to the paddle center
                const float paddleHalfWidth = colliderBounds.size.x * 0.5f;
                const float paddleCenter = colliderBounds.position.x + paddleHalfWidth;
                const float hitOffset = contactPoint.x - paddleCenter;

                // Normalize to range [-1, 1]
                const float normalizedHit = std::clamp(hitOffset / paddleHalfWidth, -1.0f, 1.0f);

                // Create deflection based on hit position
                constexpr float DEFLECTION_STRENGTH = 0.5f;

                // Modify the normal to add horizontal deflection
                sf::Vector2f modifiedNormal = normal;
                modifiedNormal.x += normalizedHit * DEFLECTION_STRENGTH;
                modifiedNormal = modifiedNormal.normalized();

                // Reflect with modified normal
                ballVelocity.velocity = CalculateReflection(ballVelocity.velocity, modifiedNormal);

                // Get the game session and reset the multiplier
                world.get_mut<Multiplier>().multiplier = 1;
            }
            else
            {
                // Reflect velocity along collision normal
                ballVelocity.velocity = CalculateReflection(ballVelocity.velocity, normal);

                if (blockEntity.has<Indestructible>())
                {
                    return;
                }

                // Remove one health from the block
                auto& [health] = blockEntity.get_mut<Health>();
                health -= 1;

                if (health <= 0)
                {
                    // Destroy the block
                    blockEntity.destruct();

                    // Update the score
                    int& multiplier = world.get_mut<Multiplier>().multiplier;
                    auto& score = world.get_mut<Score>();
                    score.score += 100 * multiplier;
                    score.blocksDestroyed += 1;
                    multiplier += 1;

                    // Trigger a shake (e.g., when something explodes)
                    world.entity().set<CameraShakeIntent>({.intensity = 3.0f, .duration = 0.3f});

                    // Emit particles
                    const auto blockCenterPosition = blockEntity.get<Transform>().position +
                                                     blockEntity.get<Size>().size * 0.5f;
                    world.entity()
                        .set<ParticleEmitter>({.ratePerSecond = 500.f})
                        .set<Lifetime>({0.2})
                        .set<Transform>({.position = blockCenterPosition});
                    world.entity()
                        .set<ParticleEmitter>({.ratePerSecond = 50.f})
                        .set<Lifetime>({0.1})
                        .set<Transform>({.position = blockCenterPosition});
                }
            }
        }
    );
}
} // namespace

void CollisionDetectionSystem::Register(const flecs::world& world)
{
    world.system<const Transform, const Size, const Origin, const ColliderShape>("CollisionDetectionSystem")
        .with<GameStatePlaying>()
        .singleton()
        .each(ProcessCollisionDetection);
}
