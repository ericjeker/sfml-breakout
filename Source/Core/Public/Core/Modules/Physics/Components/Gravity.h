// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "Core/PhysicsConstants.h"


struct Gravity
{
    sf::Vector2f gravity = PhysicsConstants::GRAVITY_DOWN;
};


#endif
