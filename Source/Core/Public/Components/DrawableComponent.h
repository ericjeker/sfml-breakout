// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Scenes/Component.h"

#include <SFML/Graphics/Drawable.hpp>

#include <memory>


struct DrawableComponent final : Component
{
    std::unique_ptr<sf::Drawable> drawable;
    bool isVisible = true;
};


#endif
