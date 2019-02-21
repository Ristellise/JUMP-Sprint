#include "AudioHandler.h"



SoundContainer::SoundContainer()
{
}


SoundContainer::~SoundContainer()
{
}

bool SoundContainer::Load(std::string filename, SourceType srcType)
{
    switch (srcType)
    {
    case ST_WAVSTREAM:
        this->trackStream.load(filename.c_str());
        break;
    case ST_WAV:
        this->track.load(filename.c_str());
        break;
    default:
        break;
    }
    return true;
}

void SoundContainer::setHandle(unsigned int handle)
{
    this->Sourcehandle = handle;
}

SoLoud::WavStream SoundContainer::get()
{
    return this->trackStream;
}
