// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BALL_H
#define BALL_H

#include "PhysicsConstants.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <flecs.h>

namespace Prefabs
{

struct BallParams
{
    float radius = 4.f;
    sf::Color color = sf::Color::Magenta;
    sf::Vector2f origin = {0.5f, 0.5f};
    sf::Vector2f position = {0.f, 0.f};
    sf::Vector2f scale = {1.f, 1.f};
    float zOrder = 0.f;

    // TODO: should have an embedded struct for physics parameters
    sf::Vector2f acceleration = {0.f, 0.f};
    sf::Vector2f velocity = {0.f, 0.f};
    sf::Vector2f gravity = PhysicsConstants::GRAVITY_DOWN;
    float mass = 1.f;
    float restitution = 0.f;
    float friction = 0.f;
};


struct Ball
{
    static flecs::entity Create(const flecs::world& world, const BallParams& params);
};


} // namespace Prefabs

#endif
