// Copyright (c) Eric Jeker 2025.

#include "CalculateFpsSystem.h"

#include "Modules/Debug/Tags/FPSWidget.h"
#include "Scenes/Debug/DebugScene.h"

#include "Core/Modules/Render/Components/TextRenderable.h"

namespace
{

auto Update()
{
    return [](const flecs::iter& it, size_t, const TextRenderable& textRenderable) {
        static float sinceLastUpdate = 0.f;
        static int frameCount = 0;
        const float deltaTime = it.world().delta_time();
        sinceLastUpdate += deltaTime;
        frameCount++;

        if (sinceLastUpdate >= .3f)
        {
            const float averageFps = std::ceil(frameCount / sinceLastUpdate);
            textRenderable.text->setString("FPS: " + std::to_string(static_cast<int>(averageFps)));
            sinceLastUpdate = 0.f;
            frameCount = 0;
        }
    };
}

} // namespace

void CalculateFpsSystem::Register(const flecs::world& world)
{
    world.system<const TextRenderable>("Debug::CalculateFPSSystem").with<const FPSWidget>().each(Update());
}
