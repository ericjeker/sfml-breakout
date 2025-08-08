// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Window.hpp>

struct MouseReleased
{
    sf::Vector2i position;
    sf::Mouse::Button button;
};
