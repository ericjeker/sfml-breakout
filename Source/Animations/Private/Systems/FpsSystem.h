// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef FPSSYSTEM_H
#define FPSSYSTEM_H
#include "Systems/System.h"


class FpsSystem final : public System {
public:
    void Update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) override;
};


#endif
