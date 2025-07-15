// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "Scenes/Scene.h"


class DemoScene final : public Scene
{
public:
    DemoScene() = default;
    void Initialize() override;
};


#endif
