// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Command.h"

#include <vector>


struct CommandQueue
{
    std::vector<Command> intents{};
};
