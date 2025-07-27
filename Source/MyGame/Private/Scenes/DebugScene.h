// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DEBUGSCENE_H
#define DEBUGSCENE_H

#include "Components/TextRenderable.h"
#include "Components/Transform.h"
#include "Scenes/Scene.h"


class DebugScene final : public Scene
{
public:
    void Initialize() override;
    static void ProcessText(const flecs::iter& it, size_t, const Transform& t, const TextRenderable& textRenderable);
};


#endif
