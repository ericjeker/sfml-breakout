// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/AudioManager.h"

AudioManager::AudioManager(ResourceManager& resourceManager)
    : _resourceManager(resourceManager)
{
}

void AudioManager::SetMasterVolume(const float volume)
{
    _masterVolume = volume;
    if (_currentlyPlaying)
    {
        SetMusicVolume(_musicVolume);
    }
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

void AudioManager::PlayMusic(const std::string& musicName)
{
    StopMusic();

    _currentlyPlaying = _resourceManager.GetResource<sf::Music>(musicName);
    if (_currentlyPlaying)
    {
        _currentlyPlaying->setVolume(_musicVolume * _masterVolume);
        _currentlyPlaying->play();
    }
}

void AudioManager::StopMusic() const
{
    if (_currentlyPlaying)
    {
        _currentlyPlaying->stop();
    }
}

void AudioManager::PauseMusic() const
{
    if (_currentlyPlaying)
    {
        _currentlyPlaying->pause();
    }
}

void AudioManager::ResumeMusic() const
{
    if (_currentlyPlaying)
    {
        _currentlyPlaying->play();
    }
}

bool AudioManager::IsMusicPlaying() const
{
    if (_currentlyPlaying)
    {
        return _currentlyPlaying->getStatus() == sf::Music::Status::Playing;
    }
    return false;
}

bool AudioManager::IsMusicPaused() const
{
    if (_currentlyPlaying)
    {
        return _currentlyPlaying->getStatus() == sf::Music::Status::Paused;
    }
    return false;
}

bool AudioManager::IsMusicStopped() const
{
    if (_currentlyPlaying)
    {
        return _currentlyPlaying->getStatus() == sf::Music::Status::Stopped;
    }
    return true;
}

void AudioManager::SetMusicVolume(const float volume)
{
    _musicVolume = volume;
    if (_currentlyPlaying)
    {
        _currentlyPlaying->setVolume(_musicVolume * _masterVolume);
    }
}

float AudioManager::GetMusicVolume() const
{
    return _musicVolume;
}

void AudioManager::SetMusicLoop(const bool loop) const
{
    if (_currentlyPlaying)
    {
        _currentlyPlaying->setLooping(loop);
    }
}

bool AudioManager::IsMusicLooping() const
{
    if (_currentlyPlaying)
    {
        return _currentlyPlaying->isLooping();
    }
    return false;
}
