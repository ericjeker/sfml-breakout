// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H
#include <cstdint>


struct Collider {
    float radius;
    std::uint32_t layers;
    bool isTrigger;
};


#endif
