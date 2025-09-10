// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/GameStates/GameState.h"
#include "Core/Managers/SceneManager.h"


class GameplayState final : public GameState
{
public:
    explicit GameplayState(flecs::world& world);
    void Enter() override;
    void Exit() override;
};

