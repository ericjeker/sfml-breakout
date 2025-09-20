// Copyright (c) Eric Jeker 2025.

#pragma once

#include <flecs.h>
#include <functional>


struct Event
{
    std::function<void(const flecs::world&)> callback;
};
