// Copyright (c) Eric Jeker 2025.

#include "Core/Utils/Collision.h"

#include <algorithm>
#include <tracy/Tracy.hpp>

namespace Collision
{

CollisionInfo CheckAABBCircleCollision(const sf::FloatRect& aabb, const sf::Vector2f& circleCenter, const float circleRadius)
{
    ZoneScopedN("CheckAABBCircleCollision");

    CollisionInfo info;

    // Find the closest point on the rectangle to the circle center
    sf::Vector2f closestPoint;
    closestPoint.x = std::clamp(circleCenter.x, aabb.position.x, aabb.position.x + aabb.size.x);
    closestPoint.y = std::clamp(circleCenter.y, aabb.position.y, aabb.position.y + aabb.size.y);

    // Calculate the distance between the circle center and the closest point
    const sf::Vector2f distance = circleCenter - closestPoint;
    const float distanceLength = distance.length();

    // Collision occurs if the distance is less than radius
    if (distanceLength > circleRadius)
    {
        // No contact, we return early
        return info;
    }

    info.hasCollision = true;
    info.penetrationDepth = circleRadius - distanceLength;

    if (distanceLength > 0.f)
    {
        info.normal = distance.normalized();
        info.contactPoint = closestPoint;

        return info;
    }

    // Special case if the distance is zero
    // Use the shortest distance to an edge as the normal
    const float leftDist = circleCenter.x - aabb.position.x;
    const float rightDist = (aabb.position.x + aabb.size.x) - circleCenter.x;
    const float topDist = circleCenter.y - aabb.position.y;
    const float bottomDist = (aabb.position.y + aabb.size.y) - circleCenter.y;

    const float minDist = std::min({leftDist, rightDist, topDist, bottomDist});

    if (minDist == leftDist)
    {
        info.normal = {-1.f, 0.f};
    }
    else if (minDist == rightDist)
    {
        info.normal = {1.f, 0.f};
    }
    else if (minDist == topDist)
    {
        info.normal = {0.f, -1.f};
    }
    else
    {
        info.normal = {0.f, 1.f};
    }

    info.contactPoint = circleCenter + info.normal * circleRadius;

    return info;
}


} // namespace Collision
