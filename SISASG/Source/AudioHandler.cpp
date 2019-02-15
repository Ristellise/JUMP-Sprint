#include "AudioHandler.h"



AudioSource::AudioSource()
{
}


AudioSource::~AudioSource()
{
}

bool AudioSource::playTrack()
{
    if (this->audiotype == AudioType::AT_2D)
    {
        this->Sourcehandle = instance->play(this->track);
    }
    else if (this->audiotype == AudioType::AT_3D)
    {
        this->Sourcehandle = instance->play3d(this->track)
    }
    return true;
}

void AudioSource::setPosition(Vector3 pos)
{
    if (this->audiotype)
    {
        this->instance->set3dSourcePosition(this->Sourcehandle,
            pos.x, pos.y, pos.z);
        this->instance->update3dAudio();
    }
    else
    {
        std::cout << "Failed to updated instance!" << std::endl;
    }
}
