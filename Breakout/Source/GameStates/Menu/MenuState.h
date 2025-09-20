// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/GameStates/GameState.h"


class MenuState final : public GameState
{
public:
    explicit MenuState(flecs::world& world);
    void Enter() override;
    void Exit() override;
};



