// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "Components/Transform.h"
#include "Modules/Physics/Components/RigidBody.h"
#include "Modules/Physics/Components/Velocity.h"
#include "Modules/Render/Components/CircleRenderable.h"
#include "Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    void CreateBalls();
    static void ProcessScreenBounce(Transform& t, Velocity& v);
    static void ProcessPhysics(const flecs::iter& it, size_t, Transform& t, Velocity& v, const RigidBody& p, CircleRenderable& ball);
};


#endif
