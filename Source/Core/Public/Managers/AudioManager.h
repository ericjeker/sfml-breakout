// Copyright (c) Eric Jeker 2025.

#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "ResourceManager.h"

#include <string>


constexpr float DEFAULT_MASTER_VOLUME = 1.0f;
// SFML max volume is 100, we'll multiply by the master volume when playing
constexpr float DEFAULT_SOUND_VOLUME = 100.0f;
constexpr float DEFAULT_MUSIC_VOLUME = 100.0f;

class AudioManager
{
public:
    explicit AudioManager(ResourceManager& resourceManager);
    ~AudioManager() = default;

    void SetMasterVolume(const float volume);
    float GetMasterVolume() const;

    void PlaySound(const std::string& soundName);
    void SetVolume(const float volume);
    float GetVolume() const;

    void PlayMusic(const std::string& musicName) const;
    void StopMusic() const;
    void PauseMusic() const;
    void ResumeMusic() const;
    bool IsMusicPlaying() const;
    bool IsMusicPaused() const;
    bool IsMusicStopped() const;
    void SetMusicVolume(const float volume);
    float GetMusicVolume() const;
    void SetMusicLoop(const bool loop) const;
    bool IsMusicLooping() const;
    void NextMusicTrack();
    void PreviousMusicTrack();

private:
    float _masterVolume = DEFAULT_MASTER_VOLUME;
    float _soundVolume = DEFAULT_SOUND_VOLUME;
    float _musicVolume = DEFAULT_MUSIC_VOLUME;

    ResourceManager& _resourceManager;
    std::shared_ptr<sf::Music> _currentlyPlaying;

    // TODO: Not Implemented
    int _currentMusicTrack = 0;
    std::vector<std::shared_ptr<sf::Music>> _musicTracks;
};


#endif
