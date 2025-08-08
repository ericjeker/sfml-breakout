// Copyright (c) Eric Jeker 2025.

#pragma once


#include "../../Scenes/ControllerDemo/ControllerDemoScene.h"
#include "../../Scenes/Pause/PauseScene.h"
#include "Core/GameStates/GameState.h"
#include "Core/Managers/EventManager.h"
#include "Core/Managers/GameService.h"
#include "Core/Managers/SceneManager.h"
#include "Events/ResumeGame.h"


class GameplayState final : public GameState
{
public:
    explicit GameplayState(flecs::world& world);
    void Enter() override;
    void Exit() override;

    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    EventListener<ResumeGame> _resumeGameListener = [this](const ResumeGame& event, void* sender)
    {
        GameService::Get<SceneManager>().UnloadScene<PauseScene>();
        GameService::Get<SceneManager>().GetScene<ControllerDemoScene>().Resume();
    };
};

