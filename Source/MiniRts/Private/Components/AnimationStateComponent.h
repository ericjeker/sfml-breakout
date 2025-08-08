// Copyright (c) Eric Jeker 2025.

#pragma once


#include "Scenes/Component.h"


struct AnimationStateComponent : Component
{
    int currentAnimation = 0;
    int currentFrame = 0;
    float animationTime = 0.f;
    float animationSpeed = 1.f;
    bool isPlaying = false;
    bool isLooping = false;
    bool isPaused = false;
    bool isFinished = false;
};



