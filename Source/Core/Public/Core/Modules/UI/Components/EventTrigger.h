// Copyright (c) Eric Jeker 2025.

#pragma once



#include <functional>


struct EventTrigger
{
    std::function<void()> callback;
};



