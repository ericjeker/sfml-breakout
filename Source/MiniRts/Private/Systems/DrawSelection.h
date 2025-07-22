// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef DRAWSELECTION_H
#define DRAWSELECTION_H

#include "Systems/System.h"


class DrawSelection final : public System {
    void Render(const std::unique_ptr<Entity>& entity, sf::RenderWindow& window) override;
};


#endif
