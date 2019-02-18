#pragma once
#include <soloud.h>
#include <soloud_wavstream.h>
#include <string>
#include <Vector3.h>
enum AudioType
{
    AT_2D,
    AT_2DCLOCK,
    AT_3D,
    AT_3DCLOCK,
};

class AudioSource
{
private:
    SoLoud::Soloud* instance;
    Vector3 *pos;
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

