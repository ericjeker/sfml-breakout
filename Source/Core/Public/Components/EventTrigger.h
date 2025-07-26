// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef EVENTTRIGGER_H
#define EVENTTRIGGER_H

#include <functional>


struct EventTrigger
{
    std::function<void()> callback;
};


#endif
