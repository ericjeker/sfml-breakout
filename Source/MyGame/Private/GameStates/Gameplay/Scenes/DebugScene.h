// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "Core/Scenes/Scene.h"


struct FPS
{
};

class DebugScene final : public Scene
{
public:
    explicit DebugScene(flecs::world& world);
    void Initialize() override;
};


#endif
