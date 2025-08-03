// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef CONTROLLERDEMOSCENE_H
#define CONTROLLERDEMOSCENE_H

#include "Scenes/Scene.h"



class ControllerDemoScene final : public Scene
{
public:
    void Initialize() override;

private:
    static void CreateInputBindings(const flecs::world& world);
    static void CreateMovementSystem(const flecs::world& world);
    static void CreateUISystem(const flecs::world& world);
    static void CreatePlayerEntity(const flecs::world& world);

};


#endif
