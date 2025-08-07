// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SFML_SANDBOX_008_ANIMATIONS_SCENEMODULE_H
#define SFML_SANDBOX_008_ANIMATIONS_SCENEMODULE_H

#include <flecs.h>

namespace Modules
{

struct SceneModule
{
    explicit SceneModule(const flecs::world& world);
};

}

#endif
