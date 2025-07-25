// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <flecs.h>


struct BallRenderable
{
    std::unique_ptr<sf::CircleShape> shape;
};

struct BackgroundRenderable
{
    std::unique_ptr<sf::RectangleShape> shape;
};

class BouncingBallScene final : public Scene
{
public:
    BouncingBallScene() = default;

    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

private:
    void CreateBalls(int count);
    static void ProcessScreenBounce(const flecs::iter& it, size_t, Transform& t, Velocity& v);
    static void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p);
};


#endif
