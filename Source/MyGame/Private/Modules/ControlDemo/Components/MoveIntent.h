// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MOVEINTENT_H
#define MOVEINTENT_H

#include <SFML/System/Vector2.hpp>


struct MoveIntent
{
    sf::Vector2f accelerate = {0.f, 0.f};
};


#endif
