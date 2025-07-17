// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H
#include "Scenes/Component.h"

struct FpsComponent : Component
{
    int fps = 0;
    float updateEvery = .3f;
    float timeSinceLastUpdate = 0.f;
};

#endif
