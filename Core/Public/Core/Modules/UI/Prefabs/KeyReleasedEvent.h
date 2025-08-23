// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Window/Event.hpp>


namespace Prefabs
{
struct KeyReleasedEvent
{
    sf::Keyboard::Key code{};
    sf::Keyboard::Scancode scancode{};
    bool alt{};
    bool control{};
    bool shift{};
    bool system{};
};
} // namespace Prefabs
