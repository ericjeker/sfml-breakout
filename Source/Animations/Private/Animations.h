// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "GameInstance.h"


class Animations final : public GameInstance
{
public:
    using GameInstance::GameInstance;
    void Initialize() override;
    void Shutdown() override;
};


#endif
