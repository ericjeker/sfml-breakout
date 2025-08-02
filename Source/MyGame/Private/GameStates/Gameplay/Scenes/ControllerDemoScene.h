// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef CONTROLLERDEMOSCENE_H
#define CONTROLLERDEMOSCENE_H

#include "Modules/Control/Components/Action.h"
#include "Modules/Control/ControlModule.h"
#include "Scenes/Scene.h"


class ControllerDemoScene final : public Scene
{
public:
    void Initialize() override;

private:
    std::unordered_map<InputKey, PlayerAction, InputKeyHash> _inputBindings;
    std::unordered_map<PlayerAction, std::function<void()>> _actionMap;
};


#endif
