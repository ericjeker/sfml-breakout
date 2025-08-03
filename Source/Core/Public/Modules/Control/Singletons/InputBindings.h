// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef INPUTBINDINGS_H
#define INPUTBINDINGS_H

#include "Modules/Control/InputKey.h"

#include <unordered_map>

struct InputBindings
{
    std::unordered_map<InputKey, flecs::entity, InputKeyHash> map;
};


#endif
