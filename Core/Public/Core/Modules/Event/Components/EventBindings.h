// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Modules/Input/InputKey.h"

#include <flecs.h>
#include <unordered_map>

struct EventBindings
{
    std::unordered_map<InputKey, flecs::entity, InputKeyHash> map{};
};



