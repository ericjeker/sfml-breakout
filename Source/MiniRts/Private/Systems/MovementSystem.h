// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "Systems/System.h"


class MovementSystem final : public System {
    void Update(const std::unique_ptr<Entity>& entity, float deltaTime) override;
};


#endif
