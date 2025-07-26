// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "../Components/BallRenderable.h"
#include "Components/Physics/RigidBody.h"
#include "Components/Physics/Velocity.h"
#include "Components/Transform/Transform.h"
#include "Scenes/Scene.h"

#include <flecs.h>

class BouncingBallScene final : public Scene
{
public:
    BouncingBallScene() = default;

    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) const;
    void Render(sf::RenderWindow& window) override;

private:
    void CreateBalls(int count);
    static void ProcessScreenBounce(const flecs::iter& it, size_t, Transform& t, Velocity& v);
    static void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p);
    static void RenderBalls(const flecs::iter& it, size_t, const Transform& t, const BallRenderable& b);
};


#endif
