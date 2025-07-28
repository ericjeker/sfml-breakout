// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SHADERRENDERABLE_H
#define SHADERRENDERABLE_H

#include <SFML/Graphics/Shader.hpp>


struct ShaderRenderable
{
    sf::Shader shader;
    sf::Drawable* target;
    sf::Shader::Type type;
};


#endif
