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
    SoLoud::handle Sourcehandle;
    SoLoud::WavStream trackStream;
    SoLoud::Wav track;
    SoLoud::Openmpt mpttrack;
    SoLoud::Soloud* loudPtr;
    bool playing = false;
public:
    void enableLooping() { this->loudPtr->setLooping(this->Sourcehandle, 1); };
    void disableLooping() { this->loudPtr->setLooping(this->Sourcehandle, 0); };
    SourceType srcType;
    SoundContainer();
    SoundContainer(SoLoud::Soloud* loudPtr);
    SoundContainer(SoLoud::Soloud* loudPtr, std::string filename, SourceType srcType = SourceType::ST_NORMAL);
    ~SoundContainer();
    bool Load(std::string filename, SourceType srcType = SourceType::ST_NORMAL);
    void play(bool background = false, bool PausedInital = false);
    void stop(bool Now = true, float timeToLive = 2.0f);
    void updatePos(const Vector3 pos);
    void loopPos(const float loopPoint);
    bool isPlaying();
    void pause();
    bool DIE(bool Now, float timeToLive = 1.0f);
    void SetLoudInstance(SoLoud::Soloud* loudPtr);
    // SoLoud::Wav get();
};

