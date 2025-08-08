// Copyright (c) Eric Jeker 2025.

#pragma once



#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <variant>

/**
 * Supported types:
 *
 * float (GLSL type float)
 * 2 floats, sf::Vector2f (GLSL type vec2)
 * 3 floats, sf::Vector3f (GLSL type vec3)
 * sf::Color (GLSL type vec4)
 * sf::Transform (GLSL type mat4)
 * sf::Texture (GLSL type sampler2D)
 */
struct ShaderUniform
{
    std::string name;
    std::variant<float, sf::Vector2f, sf::Vector3f, sf::Color, sf::Transform, const sf::Texture*> value;
};



