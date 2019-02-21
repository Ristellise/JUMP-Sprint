#pragma once
#include <soloud.h>
#include <soloud_wavstream.h>
#include <soloud_wav.h>
#include <soloud_openmpt.h>
#include <string>
#include <Vector3.h>
enum SourceType
{
    ST_STREAM,
    ST_NORMAL,
    ST_OPENMPT,

};

class SoundContainer
{
private:
    SoLoud::Soloud* instance;
    SoLoud::handle Sourcehandle;
    SoLoud::WavStream trackStream;
    SoLoud::Wav track;
    SoLoud::Openmpt mpttrack;
    SoLoud::Soloud* loudPtr;
    bool playing = false;
public:

    SourceType srcType;
    SoundContainer();
    SoundContainer(SoLoud::Soloud* loudPtr);
    SoundContainer(SoLoud::Soloud* loudPtr, std::string filename, SourceType srcType = SourceType::ST_NORMAL);
    ~SoundContainer();
    bool Load(std::string filename, SourceType srcType = SourceType::ST_NORMAL);
    void play(bool background = false);
    void stop(bool Now = true, float timeToLive = 2.0f);
    void updatePos(const Vector3 pos);
    bool isPlaying();
    bool DIE(bool Now, float timeToLive = 1.0f);
    void SetLoudInstance(SoLoud::Soloud* loudPtr);
    // SoLoud::Wav get();
};

