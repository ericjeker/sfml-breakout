// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "Scenes/Scene.h"


struct FPS
{
};

class DebugScene final : public Scene
{
public:
    void Initialize() override;
};


#endif
