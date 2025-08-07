// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Core/Scenes/Scene.h"

class MainMenuScene final : public Scene
{
public:
    explicit MainMenuScene(flecs::world& world);

    void Initialize() override;
};


#endif
