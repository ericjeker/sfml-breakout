// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef CONTROLLERDEMOSCENE_H
#define CONTROLLERDEMOSCENE_H

#include "Core/Scenes/Scene.h"


class ControllerDemoScene final : public Scene
{
public:
    explicit ControllerDemoScene(flecs::world& world);
    void Initialize() override;

private:
    static void CreateInputBindings(const flecs::world& world);
    void CreateMovementSystem(const flecs::world& world);
    void CreateUISystem(const flecs::world& world);
    void CreatePlayerEntity(const flecs::world& world);

};


#endif
