// Copyright (c) Eric Jeker 2025.

#pragma once



#include <SFML/Graphics/Text.hpp>

#include <memory>


struct TextRenderable
{
    std::unique_ptr<sf::Text> text = nullptr;
};



