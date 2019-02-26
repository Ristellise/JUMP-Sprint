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
    ST_UNSUPPORTED,
    ST_COUNT

};

class SoundContainer
{
private:
    SoLoud::handle Sourcehandle;
    SoLoud::WavStream trackStream;
    SoLoud::Wav track;
    SoLoud::Openmpt mpttrack;
    SoLoud::Soloud* loudPtr;
    Vector3* pos;
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
    void play3d(bool PausedInital = false);
    void stop(bool Now = true, float timeToLive = 2.0f);
    void updatePos(Vector3 *pos);
    void loopPos(const float loopPoint);
    bool isPlaying();
    void pause();
    bool DIE(bool Now, float timeToLive = 1.0f);
    void SetLoudInstance(SoLoud::Soloud* loudPtr);
    // SoLoud::Wav get();
};

std::string fileNameNoExt(std::string filename);

SourceType srcTypeFromExtension(std::string filename, bool stream = false);
