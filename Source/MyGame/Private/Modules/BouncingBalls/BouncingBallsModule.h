// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef BOUNCINGBALLSMODULE_H
#define BOUNCINGBALLSMODULE_H

#include <flecs.h>

namespace Modules
{

struct BouncingBallsModule
{
    explicit BouncingBallsModule(const flecs::world& world);
};

}; // namespace Modules


#endif
