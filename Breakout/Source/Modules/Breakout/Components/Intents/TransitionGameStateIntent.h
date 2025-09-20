// Copyright (c) Eric Jeker 2025.

#pragma once


enum class GameTransitions
{
    OpenMenu,
    StartPlaying,
    ResumePlaying,
    RestartPlaying,
    GameLost,
    GameWon,
    PauseGame,
};

struct TransitionGameStateIntent
{
    GameTransitions state;
};
