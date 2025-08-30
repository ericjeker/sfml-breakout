// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Graphics/Vertex.hpp>


struct CameraShake
{
    float intensity;
    float remainingTime;
    sf::Vector2f originalCenter;
    sf::Vector2f currentOffset;
};
