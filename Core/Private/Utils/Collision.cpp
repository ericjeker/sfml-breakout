// Copyright (c) Eric Jeker 2025.

#include "Core/Utils/Collision.h"

#include <algorithm>

namespace Collision
{

ContactInformation CheckAABBCircleCollision(const sf::FloatRect& aabb, const sf::Vector2f& circleCenter, const float circleRadius)
{
    ContactInformation contactInfo;

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
        return contactInfo;
    }

    contactInfo.hasCollision = true;
    contactInfo.penetrationDepth = circleRadius - distanceLength;

    if (distanceLength > 0.f)
    {
        contactInfo.normal = distance.normalized();
        contactInfo.contactPoint = circleCenter - contactInfo.normal * circleRadius;

        return contactInfo;
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
        contactInfo.normal = {-1.f, 0.f};
    }
    else if (minDist == rightDist)
    {
        contactInfo.normal = {1.f, 0.f};
    }
    else if (minDist == topDist)
    {
        contactInfo.normal = {0.f, -1.f};
    }
    else
    {
        contactInfo.normal = {0.f, 1.f};
    }

    contactInfo.contactPoint = circleCenter - contactInfo.normal * circleRadius;

    return contactInfo;
}


} // namespace Collision
