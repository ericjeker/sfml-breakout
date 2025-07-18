// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef CLEANUPSYSTEM_H
#define CLEANUPSYSTEM_H

#include "Systems/System.h"


/**
 * Clean up the entities that are not moving anymore
 */
class CleanUpSystem final : public System
{
    void Update(const std::unique_ptr<Entity>& entity, float deltaTime) override;
};


#endif
