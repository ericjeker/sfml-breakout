// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Components/Transform.h"
#include "Modules/Render/Components/RectangleRenderable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Scenes/Scene.h"

#include <SFML/Graphics/Text.hpp>

class MainMenuScene final : public Scene
{
public:
    void Initialize() override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    void CreateTextEntity(sf::Text& text, sf::Vector2f position);
    void CreateButtonEntity(sf::Text& text, sf::Vector2f position, const std::function<void()>& callback);
    static void ApplyTransform(const Transform& t, TextRenderable& textRenderable);
    static void ProcessBackground(const Transform& t, RectangleRenderable& bg);
};


#endif
