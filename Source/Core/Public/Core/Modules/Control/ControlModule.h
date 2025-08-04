// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

#include <flecs.h>


namespace Modules
{

struct ControlModule
{
    explicit ControlModule(const flecs::world& world);
};

} // namespace Modules


#endif
