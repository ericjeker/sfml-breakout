// Copyright (c) Eric Jeker 2025.

#pragma once

#include <functional>


struct Event
{
    std::function<void(const flecs::world& world)> callback;
};
