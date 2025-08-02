// Copyright (c) Eric Jeker 2025.

#include "ControllerDemoScene.h"

#include "Configuration.h"
#include "Logger.h"
#include "Managers/GameService.h"
#include "Modules/Control/Components/Action.h"
#include "Modules/Control/Components/ActionRegistry.h"
#include "Modules/Control/Components/PossessedByPlayer.h"
#include "Modules/Control/ControlModule.h"
#include "Modules/Physics/Components/Acceleration.h"
#include "Modules/Physics/Components/Friction.h"
#include "Modules/Physics/Components/Gravity.h"
#include "Modules/Physics/Components/Velocity.h"
#include "Modules/Physics/PhysicsModule.h"
#include "Modules/Render/Prefabs/Rectangle.h"
#include "Modules/Render/RenderModule.h"


void ControllerDemoScene::Initialize()
{
    _inputBindings[InputKey::Keyboard(sf::Keyboard::Key::W)] = PlayerAction::MoveUp;
    _inputBindings[InputKey::Keyboard(sf::Keyboard::Key::A)] = PlayerAction::MoveLeft;
    _inputBindings[InputKey::Keyboard(sf::Keyboard::Key::S)] = PlayerAction::MoveDown;
    _inputBindings[InputKey::Keyboard(sf::Keyboard::Key::D)] = PlayerAction::MoveRight;
    _inputBindings[InputKey::Joystick(0, sf::Joystick::Axis::X, -1, 8)] = PlayerAction::MoveLeft;
    _inputBindings[InputKey::Joystick(0, sf::Joystick::Axis::X, 1, 8)] = PlayerAction::MoveRight;
    _inputBindings[InputKey::Joystick(0, sf::Joystick::Axis::Y, -1, 8)] = PlayerAction::MoveUp;
    _inputBindings[InputKey::Joystick(0, sf::Joystick::Axis::Y, 1, 8)] = PlayerAction::MoveDown;

    _actionMap[PlayerAction::MoveUp] = [&]() {};
    _actionMap[PlayerAction::MoveLeft] = [&]() {};
    _actionMap[PlayerAction::MoveDown] = [&]() {};
    _actionMap[PlayerAction::MoveRight] = [&]() {};

    if (sf::Joystick::isConnected(0))
    {
        LOG_DEBUG("(ControllerDemoScene:Initialize): Joystick Connected");
        sf::Joystick::update();
        for (int i = 0; i < sf::Joystick::getButtonCount(0); ++i)
        {
            LOG_DEBUG(std::format("(ControllerDemoScene:Initialize): Button: {}", i));
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::X))
        {
            LOG_DEBUG("(ControllerDemoScene:Initialize): Axis: X");
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::Y))
        {
            LOG_DEBUG("(ControllerDemoScene:Initialize): Axis: Y");
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::PovX))
        {
            LOG_DEBUG("(ControllerDemoScene:Initialize): Axis: PovX");
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::PovY))
        {
            LOG_DEBUG("(ControllerDemoScene:Initialize): Axis: PovY");
        }
    }

    auto world = GetWorld();

    world.import <Modules::ControlModule>();
    world.import <Modules::PhysicsModule>();
    world.import <Modules::RenderModule>();

    constexpr float CENTER_X = Configuration::WINDOW_SIZE.x / 2;
    constexpr float CENTER_Y = Configuration::WINDOW_SIZE.y / 2;

    const flecs::entity entity = Prefabs::Rectangle::Create(
        world,
        {
            .size = {10.f, 10.f},
            .color = sf::Color::Red,
            .origin = {0.5f, 0.5f},
            .position = {CENTER_X, CENTER_Y},
            .zOrder = 0.f,
        }
    );

    entity.add<PossessedByPlayer>().set<ActionRegistry>({});
    entity.set<Acceleration>({});
    entity.set<Friction>({.friction = 0.9f});
    entity.set<Gravity>({PhysicsConstants::NO_GRAVITY});
    entity.set<Velocity>({});

    const sf::RenderWindow& window = GameService::Get<sf::RenderWindow>();

    world.system()
        .kind(flecs::PostLoad)
        .run(
            [&](flecs::iter& it)
            {
                /**
                 * TODO:
                 *   1. For each Scene, we need to create a map for sf::Event -> Action
                 *   2. For each event we receive here, we need to map it to a Command or Action
                 *   3. When we have found an Action, we search for the entity tagged with PossessedByPlayer
                 *   4. We then create the Intent component on that player with the parameters of the input
                 *   4.1. There are 2 types of input: State (ON/OFF) and Axis (vector)
                 */
                if (!IsLoaded() || IsPaused())
                {
                    return;
                }

                const auto q = GetWorld().query<ActionRegistry, const PossessedByPlayer>();

                // Loop each binding to see if they input is activated
                for (const auto& [inputKey, action] : _inputBindings)
                {
                    if (inputKey.isActivated())
                    {
                        Action intent{};
                        intent.action = action;
                        q.each([&](ActionRegistry& registry, const PossessedByPlayer& p)
                               { registry.intents.push_back(intent); });
                    }
                }
            }
        );

    // PlayerController System
    world.system<Acceleration, ActionRegistry, const PossessedByPlayer>()
        .kind(flecs::PreUpdate)
        .each(
            [&](Acceleration& a, ActionRegistry& registry, const PossessedByPlayer& p)
            {
                if (registry.intents.empty())
                {
                    return;
                }

                while (!registry.intents.empty())
                {
                    auto [action] = registry.intents.back();
                    registry.intents.pop_back();

                    switch (action)
                    {
                        case PlayerAction::MoveUp:
                        {
                            LOG_DEBUG("(ControlModule::PlayerController): PlayerAction::MoveUp");
                            a.acceleration.y -= 100.f;
                            break;
                        }
                        case PlayerAction::MoveDown:
                        {
                            LOG_DEBUG("(ControlModule::PlayerController): PlayerAction::MoveDown");
                            a.acceleration.y += 100.f;
                            break;
                        }
                        case PlayerAction::MoveLeft:
                        {
                            LOG_DEBUG("(ControlModule::PlayerController): PlayerAction::MoveLeft");
                            a.acceleration.x -= 100.f;
                            break;
                        }
                        case PlayerAction::MoveRight:
                        {
                            LOG_DEBUG("(ControlModule::PlayerController): PlayerAction::MoveRight");
                            a.acceleration.x += 100.f;
                            break;
                        }
                        default:
                        case PlayerAction::None:
                        {
                            break;
                        }
                    }
                }
            }
        );
}
