// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLFLECSSCENE_H
#define BOUNCINGBALLFLECSSCENE_H

#include "Core/Scenes/Scene.h"

#include <flecs.h>


class BouncingBallScene final : public Scene
{
public:
    void Initialize() override;

private:
    static void CreateBalls(const flecs::world& world, float zOrder);
};


#endif
