// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


class AudioManager
{
public:
    void SetVolume(const float volume) { _volume = volume; }
    float GetVolume() const { return _volume; }

private:
    float _volume = 1.0f;
};


#endif
