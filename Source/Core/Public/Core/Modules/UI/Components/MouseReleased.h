// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SFML_SANDBOX_008_ANIMATIONS_MOUSERELEASED_H
#define SFML_SANDBOX_008_ANIMATIONS_MOUSERELEASED_H

#include <SFML/Window.hpp>

struct MouseReleased
{
    sf::Vector2i position;
    sf::Mouse::Button button;
};

#endif
