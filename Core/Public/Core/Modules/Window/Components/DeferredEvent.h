// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>
#include <functional>


struct DeferredEvent
{
    std::function<void(flecs::world&)> callback;
};
