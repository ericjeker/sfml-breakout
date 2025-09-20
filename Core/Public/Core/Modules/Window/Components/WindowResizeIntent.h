// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/System/Vector2.hpp>

/**
 * @struct WindowResizeIntent
 *
 * @brief Represents an intent to resize the window with details about the new and old dimensions.
 */
struct WindowResizeIntent
{
    sf::Vector2u newSize;
    sf::Vector2u oldSize;
    float scaleRatio;
    sf::Vector2f transformRatio;
};
