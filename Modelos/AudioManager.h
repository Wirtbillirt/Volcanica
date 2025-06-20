#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <vector>
#include <AL/al.h>
#include <Al/alc.h>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool playBackgroundMusic(const std::string& path);
    void stopBackgroundMusic();
    void setBackgroundVolume(float volume);

    bool playSoundEffect(const std::string& path);
    void setEffectVolume(float volume);
    void setVolume(float volume); // 0.0f = silencio, 1.0f = volumen normal, >1.0f amplifica

private:
    ALCdevice* device;
    ALCcontext* context;

    ALuint musicBuffer;
    ALuint musicSource;

    ALuint effectBuffer;
    ALuint effectSource;
};

#endif
