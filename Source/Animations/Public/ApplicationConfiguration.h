// Copyright (c) Eric Jeker. All Rights Reserved.
#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Window.hpp"

namespace ApplicationConfiguration
{

constexpr auto windowTitle = "Animation System";
constexpr sf::Vector2u windowSize = {1920, 1080};
constexpr auto windowStyle = sf::Style::None;
constexpr auto windowState = sf::State::Fullscreen;
constexpr bool isFullscreen = false;

constexpr unsigned int framesPerSecond = 60;
constexpr bool isVSync = true;
constexpr unsigned int antiAliasingLevel = 4;

}
