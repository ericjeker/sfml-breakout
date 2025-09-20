// Copyright (c) Eric Jeker 2025.

#include "Core/GameStates/GameState.h"


GameState::GameState(flecs::world& world)
    : _world(world)
{
}

void GameState::Enter()
{
    _rootEntity = GetWorld().entity();
}

void GameState::Exit()
{
    if (GetRootEntity() == flecs::entity::null() || !GetWorld().exists(GetRootEntity()))
    {
        return;
    }

    GetWorld().defer([&] {
        GetWorld().delete_with(flecs::ChildOf, GetRootEntity());
        GetRootEntity().destruct();
    });
}

flecs::entity GameState::GetRootEntity() const
{
    return _rootEntity;
}

void GameState::Pause()
{
}

void GameState::Resume()
{
}

bool GameState::IsPaused() const
{
    return false;
}

flecs::world& GameState::GetWorld()
{
    return _world;
}

const flecs::world& GameState::GetWorld() const
{
    return _world;
}
