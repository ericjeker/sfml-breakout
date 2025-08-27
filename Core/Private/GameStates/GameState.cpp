// Copyright (c) Eric Jeker 2025.

#include "Core/GameStates/GameState.h"

#include "Core/Utils/Logger.h"

GameState::GameState(flecs::world& world)
    : _world(world)
{
}
void GameState::Enter()
{
    LOG_DEBUG("GameState::Enter");

    _rootEntity = GetWorld().entity();
}

void GameState::Exit()
{
    LOG_DEBUG("GameState::Exit");

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
    LOG_DEBUG("GameState::Pause");
}

void GameState::Resume()
{
    LOG_DEBUG("GameState::Resume");
}

bool GameState::IsPaused() const
{
}

void GameState::HandleEvent(const std::optional<sf::Event>& event)
{
}

flecs::world& GameState::GetWorld()
{
    return _world;
}

const flecs::world& GameState::GetWorld() const
{
    return _world;
}
