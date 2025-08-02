// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ACTIONREGISTRY_H
#define ACTIONREGISTRY_H

#include "Action.h"

#include <vector>


struct ActionRegistry
{
    std::vector<Action> intents{};
};


#endif
