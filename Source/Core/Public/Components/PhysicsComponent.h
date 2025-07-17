// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Scenes/Component.h"

#include <SFML/System/Vector2.hpp>


struct PhysicsComponent final : Component {
    sf::Vector2f velocity{0.f, 0.f};
    sf::Vector2f acceleration{0.f, 0.f};
    float mass = 1.f;
    float friction = 1.f;
    float restitution = 0.f;
};


#endif
