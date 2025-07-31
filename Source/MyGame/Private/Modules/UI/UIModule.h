// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef UIMODULE_H
#define UIMODULE_H

#include <flecs.h>


namespace Modules
{

struct UIModule
{
    explicit UIModule(flecs::world& world);
};

}


#endif
