// Copyright (c) Eric Jeker 2025.

#include "Managers/AudioManager.h"

AudioManager::AudioManager(ResourceManager& resourceManager)
    : _resourceManager(resourceManager)
{
}

void AudioManager::SetMasterVolume(const float volume)
{
    _masterVolume = volume;
}

float AudioManager::GetMasterVolume() const
{
    return _masterVolume;
}

void AudioManager::SetVolume(const float volume)
{
    _soundVolume = volume;
}

float AudioManager::GetVolume() const
{
    return _soundVolume;
}

void AudioManager::PlayMusic(const std::string& musicName) const
{
    const auto _currentlyPlaying = _resourceManager.GetResource<sf::Music>(musicName);
    if (!_currentlyPlaying)
    {
        LOG_ERROR("(AudioManager::PlayMusic): Could not find music: " + musicName);
        return;
    }

    _currentlyPlaying->setVolume(_musicVolume * _masterVolume);
    _currentlyPlaying->play();
}

void AudioManager::StopMusic() const
{
    if (!_currentlyPlaying)
    {
        return;
    }

    _currentlyPlaying->stop();
}

void AudioManager::PauseMusic() const
{
    if (!_currentlyPlaying)
    {
        return;
    }

    _currentlyPlaying->pause();
}

void AudioManager::ResumeMusic() const
{
    if (!_currentlyPlaying)
    {
        return;
    }

    _currentlyPlaying->play();
}

bool AudioManager::IsMusicPlaying() const
{
    if (!_currentlyPlaying)
    {
        return false;
    }

    return _currentlyPlaying->getStatus() == sf::Music::Status::Playing;
}

bool AudioManager::IsMusicPaused() const
{
    if (!_currentlyPlaying)
    {
        return false;
    }

    return _currentlyPlaying->getStatus() == sf::Music::Status::Paused;
}

bool AudioManager::IsMusicStopped() const
{
    if (!_currentlyPlaying)
    {
        return false;
    }

    return _currentlyPlaying->getStatus() == sf::Music::Status::Stopped;
}

void AudioManager::SetMusicVolume(const float volume)
{
    _musicVolume = volume;
}

float AudioManager::GetMusicVolume() const
{
    return _musicVolume;
}

void AudioManager::SetMusicLoop(const bool loop) const
{
    if (!_currentlyPlaying)
    {
        return;
    }

    _currentlyPlaying->setLooping(loop);
}

bool AudioManager::IsMusicLooping() const
{
    if (!_currentlyPlaying)
    {
        return false;
    }

    return _currentlyPlaying->isLooping();
}
