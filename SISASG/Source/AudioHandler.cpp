#include "AudioHandler.h"



SoundContainer::SoundContainer()
{
}

SoundContainer::SoundContainer(SoLoud::Soloud * loudPtr)
{
    this->SetLoudInstance(loudPtr);
}

SoundContainer::SoundContainer(SoLoud::Soloud * loudPtr, std::string filename, SourceType srcType)
{
    this->SetLoudInstance(loudPtr);
    this->Load(filename, srcType);
}


SoundContainer::~SoundContainer()
{
    this->DIE(true);
}

bool SoundContainer::Load(std::string filename, SourceType srcType)
{
    switch (srcType)
    {
    case ST_STREAM:
        this->trackStream.load(filename.c_str());
        this->srcType = SourceType::ST_STREAM;
        break;
    case ST_NORMAL:
        this->track.load(filename.c_str());
        this->srcType = SourceType::ST_NORMAL;
        break;
    case ST_OPENMPT:
        this->mpttrack.load(filename.c_str());
        this->srcType = SourceType::ST_OPENMPT;
        break;
    default:
        break;
    }
    return true;
}

void SoundContainer::play(bool background)
{
    switch (this->srcType)
    {
    case SourceType::ST_NORMAL:
    {
        if (background)
        {
            this->Sourcehandle = this->loudPtr->playBackground(this->track);
        }
        else
        {
            this->Sourcehandle = this->loudPtr->play(this->track);
        }
        break;
    }
    case SourceType::ST_STREAM:
    {
        if (background)
        {
            this->Sourcehandle = this->loudPtr->playBackground(this->trackStream);
        }
        else
        {
            this->Sourcehandle = this->loudPtr->play(this->trackStream);
            
        }
        break;
    }
    case SourceType::ST_OPENMPT:
    {
        if (background)
        {
            this->Sourcehandle = this->loudPtr->playBackground(this->mpttrack);
        }
        else
        {
            this->Sourcehandle = this->loudPtr->play(this->mpttrack);

        }
        break;
    }
    default:
        break;
    }
	this->playing = true;
}

void SoundContainer::stop(bool Now, float timeToLive)
{
    this->DIE(Now, timeToLive);
}

void SoundContainer::updatePos(const Vector3 pos)
{
    this->loudPtr->set3dSourcePosition(this->Sourcehandle, pos.x, pos.y, pos.z);
}

bool SoundContainer::isPlaying()
{
    return this->playing;
}

bool SoundContainer::DIE(bool Now, float timeToLive)
{
    if (Now)
    {
        std::cout << "Stopping Audio Now..." << std::endl;
        this->loudPtr->stop(this->Sourcehandle);
    }
    else
    {
        this->loudPtr->fadeVolume(this->Sourcehandle, 0, timeToLive);
        this->loudPtr->scheduleStop(this->Sourcehandle, timeToLive);
    }
    return true;
}

void SoundContainer::SetLoudInstance(SoLoud::Soloud * loudPtr)
{
    this->loudPtr = loudPtr;
}

