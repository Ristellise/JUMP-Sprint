#pragma once
#include <soloud.h>
#include <soloud_wavstream.h>
#include <string>
#include <Vector3.h>
enum AudioType
{
    AT_2D,
    AT_3D
};

class AudioSource
{
private:
    SoLoud::Soloud* instance;
public:

    AudioType audiotype;
    AudioSource();
    ~AudioSource();
    bool playTrack();
    bool pauseTrack();
    bool loadTrack(std::string audiofile,AudioType audioType);
    SoLoud::handle Sourcehandle;
    SoLoud::WavStream track;
    void setPosition(Vector3 pos);
};

