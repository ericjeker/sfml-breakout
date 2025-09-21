// Copyright (c) Eric Jeker 2025.

#pragma once

#include <SFML/Window.hpp>

#include <functional>


/**
 * @class InputKey
 * @brief Represents an abstraction for user input keys in a system.
 *
 * The InputKey class provides a mechanism for managing and interpreting
 * keys that are used as inputs, typically in event handling systems,
 * user interface controls, or similar contexts. This class can encapsulate
 * various attributes or characteristics of a key, such as its code, state,
 * or other metadata, offering a structured way to handle key-based inputs.
 */
struct InputKey
{
    struct Key
    {
        sf::Keyboard::Key code;

        constexpr bool operator==(const Key& rhs) const noexcept
        {
            return code == rhs.code;
        }
    };

    struct MouseBtn
    {
        sf::Mouse::Button button;

        constexpr bool operator==(const MouseBtn& rhs) const noexcept
        {
            return button == rhs.button;
        }
    };

    struct JoyButton
    {
        unsigned id{};
        unsigned button{};

        constexpr bool operator==(const JoyButton& rhs) const noexcept
        {
            return id == rhs.id && button == rhs.button;
        }
    };

    struct JoyAxisDir
    {
        unsigned id{};
        sf::Joystick::Axis axis{};
        // direction = -1 / +1 (analogue)
        int direction{};
        float deadZone{5};

        constexpr bool operator==(const JoyAxisDir& rhs) const noexcept
        {
            return id == rhs.id && axis == rhs.axis && direction == rhs.direction;
        }
    };

    // Indicate which control was used
    std::variant<Key, MouseBtn, JoyButton, JoyAxisDir> id;

    // Function that query SFML to check if the input is activated
    std::function<bool()> isActivated;

    // Constructor Helpers
    // TODO: Add chords to they keyboard keys (alt, ctrl, shift, system)
    static InputKey Keyboard(sf::Keyboard::Key key)
    {
        return {.id = Key{key}, .isActivated = [key] { return sf::Keyboard::isKeyPressed(key); }};
    }

    // TODO: Add chords to they keyboard keys (alt, ctrl, shift, system)
    static InputKey Mouse(sf::Mouse::Button button)
    {
        return {.id = MouseBtn{button}, .isActivated = [button] { return sf::Mouse::isButtonPressed(button); }};
    }

    static InputKey Joystick(unsigned id, unsigned button)
    {
        return {
            .id = JoyButton{.id = id, .button = button},
            .isActivated = [id, button]
            { return sf::Joystick::isConnected(id) && sf::Joystick::isButtonPressed(id, button); }
        };
    }

    static InputKey Joystick(unsigned id, sf::Joystick::Axis axis, int direction, float deadZone)
    {
        return {
            .id = JoyAxisDir{.id = id, .axis = axis, .direction = direction, .deadZone = deadZone},
            .isActivated =
                [id, axis, direction, deadZone]
            {
                if (!sf::Joystick::isConnected(id))
                {
                    return false;
                }

                const float axisPosition = sf::Joystick::getAxisPosition(id, axis);

                // Check if the position is below the dead zone
                if (std::abs(axisPosition) < deadZone)
                {
                    return false;
                }

                if (direction > 0 && axisPosition >= direction)
                {
                    return true;
                }

                if (direction < 0 && axisPosition <= direction)
                {
                    return true;
                }

                return false;
            }
        };
    }

    // Operator overload to be able to use InputKey in an unordered_map<InputKey, Command>
    bool operator==(const InputKey& rhs) const noexcept
    {
        return id == rhs.id;
    }
};

/**
 * @class InputKeyHash
 * @brief Provides a hashing mechanism for InputKey objects.
 *
 * The InputKeyHash class is designed to generate hash values for InputKey
 * objects, enabling their use as keys in hash-based data structures such as
 * unordered maps or hash tables. This class implements a hashing algorithm
 * tailored to ensure efficient and unique hash code generation, based on the
 * properties encapsulated in the InputKey.
 */
struct InputKeyHash
{
    std::size_t operator()(const InputKey& k) const noexcept
    {
        // Visitor for all the InputKey types
        return std::visit(
            []<typename T>(const T& v)
            {
                if constexpr (std::is_same_v<T, InputKey::Key>)
                {
                    return static_cast<std::size_t>(v.code);
                }
                else if constexpr (std::is_same_v<T, InputKey::MouseBtn>)
                {
                    return 0x1000 + static_cast<std::size_t>(v.button);
                }
                else if constexpr (std::is_same_v<T, InputKey::JoyButton>)
                {
                    return 0x2000u + (static_cast<std::size_t>(v.id) << 8) + static_cast<std::size_t>(v.button);
                }
                else
                {
                    return 0x3000u + (static_cast<std::size_t>(v.id) << 12) + (static_cast<std::size_t>(v.axis) << 1) +
                           static_cast<std::size_t>(v.direction > 0);
                }
            },
            k.id
        );
    }
};
