// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Core/Scenes/Scene.h"

#include <SFML/Graphics/Text.hpp>

class MainMenuScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;
};


#endif
