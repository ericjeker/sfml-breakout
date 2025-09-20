// Copyright (c) Eric Jeker 2025.

#pragma once

struct GameOverIntent
{
    enum class Reason
    {
        BallOutOfBounds,
        NoLivesLeft,
    } reason = Reason::BallOutOfBounds;
};
