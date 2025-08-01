// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PHYSICSMODULE_H
#define PHYSICSMODULE_H

#include <flecs.h>


namespace Modules
{

struct PhysicsModule
{
    explicit PhysicsModule(const flecs::world& world);
};

} // namespace Modules

#endif
