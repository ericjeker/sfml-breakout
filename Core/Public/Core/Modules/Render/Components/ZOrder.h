// Copyright (c) Eric Jeker 2025.

#pragma once

namespace ZOrderLayer
{
constexpr float Debug = 1000.0f;
};

/**
 * @brief Component to define the rendering order of an entity.
 * Entities with lower ZOrder values are rendered first (further back).
 */
struct ZOrder
{
    float zOrder = 0.f;
};
