// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ACTIONREGISTRY_H
#define ACTIONREGISTRY_H

#include "Command.h"

#include <vector>


struct CommandQueue
{
    std::vector<Command> intents{};
};


#endif
