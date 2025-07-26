// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MYGAME_H
#define MYGAME_H

#include "GameInstance.h"


class MyGame final : public GameInstance
{
public:
    using GameInstance::GameInstance;
    void Initialize() override;
    void Shutdown() override;
};


#endif
