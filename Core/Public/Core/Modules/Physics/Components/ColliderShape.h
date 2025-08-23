// Copyright (c) Eric Jeker 2025.

#pragma once

enum class Shape
{
    Rectangle,
    Circle
};

struct ColliderShape
{
    Shape shape = Shape::Rectangle;
};
