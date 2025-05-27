#include "AudioManager.h"
#include "WavLoader.h"
#include <iostream>

AudioManager::AudioManager() {
    device = alcOpenDevice(nullptr);
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);

    alGenBuffers(1, &musicBuffer);
    alGenSources(1, &musicSource);

    alGenBuffers(1, &effectBuffer);
    alGenSources(1, &effectSource);
}

AudioManager::~AudioManager() {
    alDeleteSources(1, &musicSource);
    alDeleteBuffers(1, &musicBuffer);
    alDeleteSources(1, &effectSource);
    alDeleteBuffers(1, &effectBuffer);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool AudioManager::playBackgroundMusic(const std::string& path) {
    WavLoader loader;
    std::vector<char> data;
    ALenum format;
    ALsizei freq;

    if (!loader.load(path, data, format, freq)) {
        std::cerr << "[AudioManager] Error al cargar música de fondo\n";
        return false;
    }

    alBufferData(musicBuffer, format, data.data(), static_cast<ALsizei>(data.size()), freq);
    alSourcei(musicSource, AL_BUFFER, musicBuffer);
    alSourcei(musicSource, AL_LOOPING, AL_TRUE);
    alSourcePlay(musicSource);

    return true;
}

void AudioManager::stopBackgroundMusic() {
    alSourceStop(musicSource);
}

void AudioManager::setBackgroundVolume(float volume) {
    alSourcef(musicSource, AL_GAIN, volume);
}

bool AudioManager::playSoundEffect(const std::string& path) {
    WavLoader loader;
    std::vector<char> data;
    ALenum format;
    ALsizei freq;

    if (!loader.load(path, data, format, freq)) {
        std::cerr << "[AudioManager] Error al cargar efecto de sonido\n";
        return false;
    }

    alBufferData(effectBuffer, format, data.data(), static_cast<ALsizei>(data.size()), freq);
    alSourcei(effectSource, AL_BUFFER, effectBuffer);
    alSourcei(effectSource, AL_LOOPING, AL_FALSE);
    alSourcePlay(effectSource);

    return true;
}

void AudioManager::setEffectVolume(float volume) {
    alSourcef(effectSource, AL_GAIN, volume);
}