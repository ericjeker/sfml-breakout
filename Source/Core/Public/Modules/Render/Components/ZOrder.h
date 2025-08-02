// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef ZORDER_H
#define ZORDER_H

/**
 * @brief Component to define the rendering order of an entity.
 * Entities with lower ZOrder values are rendered first (further back).
 */
struct ZOrder
{
    float zOrder = 0.f;
};


#endif
