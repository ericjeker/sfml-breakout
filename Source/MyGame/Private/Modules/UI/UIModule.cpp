// Copyright (c) Eric Jeker 2025.

#include "UIModule.h"

#include "Components/ButtonBackground.h"
#include "Components/ButtonText.h"
#include "Components/Clickable.h"
#include "Components/Hoverable.h"
#include "Modules/Render/Components/TextRenderable.h"
#include "Prefabs/Button.h"


namespace
{

void RenderButton(flecs::iter& it) {};

}

namespace Modules
{

UIModule::UIModule(flecs::world& world)
{
    // --- Declare Components ---
    world.component<ButtonBackground>();
    world.component<ButtonText>();
    world.component<TextRenderable>();
    world.component<Clickable>();
    world.component<Hoverable>();

    // --- Declare Systems ---
    // world.system<ButtonBackground, TextRenderable>().kind(flecs::OnStore).each(RenderButton);
}

} // namespace Modules
