// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef SHADERRENDERABLE_H
#define SHADERRENDERABLE_H

#include <SFML/Graphics/Shader.hpp>

template <typename T>
struct ShaderRenderable
{
    sf::Shader shader;
    T mesh;
    sf::Shader::Type type;
};


#endif
