// Copyright (c) Eric Jeker 2025.

#include "Scenes/Scene.h"

bool Scene::IsLoaded() const
{
    return _isLoaded;
}

bool Scene::IsPaused() const
{
    return _isPaused;
}

void Scene::Pause()
{
    _isPaused = true;
}

void Scene::Resume()
{
    _isPaused = false;
}
