// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    static void CreateBalls(const flecs::world& world);
};


#endif
