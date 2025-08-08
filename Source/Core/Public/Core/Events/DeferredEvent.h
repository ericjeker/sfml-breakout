// Copyright (c) Eric Jeker 2025.

#pragma once

#include <functional>


struct DeferredEvent
{
    std::function<void()> callback;
};
