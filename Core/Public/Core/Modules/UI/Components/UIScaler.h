// Copyright (c) Eric Jeker 2025.

#pragma once

enum class ScalingBehaviorType
{
    None,
    ScaleToFit,
    ScaleToFill
};

/**
 * @struct UIScaler
 * @brief A structure to define a scaling behavior.
 *
 * The Scaler struct represents a scaling behavior we can apply to elements when a scaling occurs, for example,
 * this will happen when the viewport is resized or the window is resized.
 */
struct UIScaler
{
    ScalingBehaviorType behavior = ScalingBehaviorType::None;
};
