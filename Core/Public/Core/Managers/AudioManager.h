// Copyright (c) Eric Jeker 2025.

#pragma once

#include "Core/Managers/ResourceManager.h"

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

    void SetMasterVolume(float volume);
    [[nodiscard]] float GetMasterVolume() const;

    void PlaySound(const std::string& soundName);
    void SetVolume(float volume);
    [[nodiscard]] float GetVolume() const;

    void PlayMusic(const std::string& musicName);
    void StopMusic() const;
    void PauseMusic() const;
    void ResumeMusic() const;
    bool IsMusicPlaying() const;
    [[nodiscard]] bool IsMusicPaused() const;
    [[nodiscard]] bool IsMusicStopped() const;
    void SetMusicVolume(float volume);
    [[nodiscard]] float GetMusicVolume() const;
    void SetMusicLoop(bool loop) const;
    [[nodiscard]] bool IsMusicLooping() const;
    void NextMusicTrack();
    void PreviousMusicTrack();

private:
    float _masterVolume = DEFAULT_MASTER_VOLUME;
    float _soundVolume = DEFAULT_SOUND_VOLUME;
    float _musicVolume = DEFAULT_MUSIC_VOLUME;

    ResourceManager& _resourceManager;
    std::shared_ptr<sf::Music> _currentlyPlaying = nullptr;

    // TODO: Not Implemented
    int _currentMusicTrack = 0;
    std::vector<std::shared_ptr<sf::Music>> _musicTracks;
};


