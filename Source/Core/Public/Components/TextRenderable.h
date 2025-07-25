// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef TEXTRENDERABLE_H
#define TEXTRENDERABLE_H

#include <SFML/Graphics/Text.hpp>

#include <memory>


struct TextRenderable
{
    std::unique_ptr<sf::Text> text;
};


#endif
