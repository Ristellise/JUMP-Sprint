#pragma once
#include <soloud.h>
#include <soloud_wavstream.h>
#include <soloud_wav.h>
#include <string>
#include <Vector3.h>
enum SourceType
{
    ST_WAVSTREAM,
    ST_WAV,
    ST_OPENMPT,

};

class SoundContainer
{
private:
    SoLoud::Soloud* instance;
public:

    SourceType srcType;
    SoundContainer();
    ~SoundContainer();
    bool Load(std::string filename, SourceType srcType = SourceType::ST_WAV);
    void setHandle(unsigned int handle);
    SoLoud::handle Sourcehandle;
    SoLoud::WavStream trackStream;
    SoLoud::Wav track;

    SoLoud::WavStream get();
    SoLoud::Wav get();
};

