// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "../Modules/RenderModule/Components/CircleShape.h"
#include "Components/RigidBody.h"
#include "Components/Transform.h"
#include "Components/Velocity.h"
#include "Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;

private:
    void CreateBalls(int count);
    static void ProcessScreenBounce(const flecs::iter& it, size_t, Transform& t, Velocity& v);
    static void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p, const CircleShape& ball);
};


#endif
