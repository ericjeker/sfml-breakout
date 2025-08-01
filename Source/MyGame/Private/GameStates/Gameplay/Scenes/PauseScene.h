// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include "Scenes/Scene.h"

#include <SFML/Graphics/Text.hpp>


class PauseScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;
};


#endif
