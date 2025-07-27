// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef GRAVITYSETTINGS_H
#define GRAVITYSETTINGS_H

#include <SFML/System/Vector2.hpp>


struct GravitySettings {
    sf::Vector2f gravity;
    float pixelsPerCentimeter;
    bool enabled = true;
};


#endif
