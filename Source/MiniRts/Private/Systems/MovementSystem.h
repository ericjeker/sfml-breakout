// Copyright (c) Eric Jeker 2025.

#pragma once



#include "Systems/System.h"


class MovementSystem final : public System {
    void Update(const std::unique_ptr<Entity>& entity, float deltaTime) override;
};



