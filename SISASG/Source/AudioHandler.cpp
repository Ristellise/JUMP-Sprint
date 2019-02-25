#include "AudioHandler.h"
#include <algorithm>


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

void SoundContainer::play(bool background, bool PausedInital)
{
    if (this == nullptr)
    {
        std::cout << "ERROR: SoundContainer is NULL. Check your code." << std::endl;
    }
    else
    {
        switch (this->srcType)
        {
        case SourceType::ST_NORMAL:
        {
            if (background)
            {
                this->Sourcehandle = this->loudPtr->playBackground(this->track, -1.0f, PausedInital);
            }
            else
            {
                this->Sourcehandle = this->loudPtr->play(this->track, -1.0f, PausedInital);
            }
            break;
        }
        case SourceType::ST_STREAM:
        {
            if (background)
            {
                this->Sourcehandle = this->loudPtr->playBackground(this->trackStream, -1.0f, PausedInital);
            }
            else
            {
                this->Sourcehandle = this->loudPtr->play(this->trackStream, -1.0f, PausedInital);

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
}

void SoundContainer::stop(bool Now, float timeToLive)
{
    this->DIE(Now, timeToLive);
}

void SoundContainer::updatePos(const Vector3 pos)
{
    this->loudPtr->set3dSourcePosition(this->Sourcehandle, pos.x, pos.y, pos.z);
}

void SoundContainer::loopPos(const float loopPoint)
{
    this->loudPtr->setLoopPoint(this->Sourcehandle, loopPoint);
}

bool SoundContainer::isPlaying()
{
    return this->playing;
}

void SoundContainer::pause()
{
    if (this->loudPtr->getPause(this->Sourcehandle))
    {
        std::cout << "Unpausing..." << std::endl;
        this->loudPtr->setPause(this->Sourcehandle, 0);
    }
    else
    {
        std::cout << "Pausing..." << std::endl;
        this->loudPtr->setPause(this->Sourcehandle, 1);
    }
    
}

bool SoundContainer::DIE(bool Now, float timeToLive)
{
    if (this == nullptr)
    {
        std::cout << "SoundContainer Was Null. Did you delete it?" << std::endl;
        return false;
    }
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

std::string fileNameNoExt(std::string filename)
{
    std::string::size_type index;
    std::string::size_type rfilename;
    rfilename = filename.find_last_of('/');
    index = filename.rfind('.');

    if (rfilename != std::string::npos)
    {
        filename = filename.substr(rfilename);
    }

    if (index != std::string::npos)
    {
        std::string ext = filename.substr(0, index);
    }
    else
    {
        return "";
    }
}

SourceType srcTypeFromExtension(std::string filename, bool stream)
{
    std::string::size_type index;
    index = filename.rfind('.');
    
    if (index != std::string::npos)
    {
        std::string ext = filename.substr(index + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == "xm" || ext == "mod" || ext == "s3m" || ext == "mitod" ||
            ext == "mptm")
        {
            return SourceType::ST_OPENMPT;
        }
        else if (ext == "flac" || ext == "wav" || ext == "ogg" || ext == "mp3")
        {
            if (stream)
            {
                return SourceType::ST_STREAM;
            }
            else
            {
                return SourceType::ST_NORMAL;
            }
        }
        else
        {
            return SourceType::ST_UNSUPPORTED;
        }
    }
    return SourceType::ST_UNSUPPORTED;
}