// Copyright (c) Eric Jeker 2025.

#include "UIModule.h"

#include "Components/ButtonBackground.h"
#include "Components/ButtonText.h"
#include "Components/Clickable.h"
#include "Components/Hoverable.h"
#include "Components/Size.h"
#include "Components/Transform.h"
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

    // --- Declare Prefabs ---
    world.prefab("ButtonPrefab")
        .set<ButtonBackground>({})
        .set<ButtonText>({.text = "Button Text"})
        .set<Transform>({})
        .set<Size>({})
        .set<Clickable>({})
        .set<Hoverable>({});

    // --- Declare Systems ---
    world.system<ButtonBackground, TextRenderable>().kind(flecs::OnStore).each(RenderButton);
}

} // namespace Modules
