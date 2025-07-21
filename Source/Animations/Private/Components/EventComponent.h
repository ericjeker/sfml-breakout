// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef EVENTEMITTERCOMPONENT_H
#define EVENTEMITTERCOMPONENT_H

#include <Scenes/Component.h>
#include <functional>

enum class TriggerType
{
    OnEnter,
    OnExit,
    OnUpdate,
    OnEvent,
    OnMousePressed
};

struct EventComponent : Component
{
    TriggerType triggerType = TriggerType::OnMousePressed;
    bool isActive = true;
    std::function<void()> callback;
};


#endif
