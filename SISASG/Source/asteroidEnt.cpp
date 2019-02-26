#include "asteroidEnt.h"
#include "AudioHandler.h"


asteroidEnt::asteroidEnt()
{
}

void asteroidEnt::OnHit(entity * Ent)
{
    if (Ent->type == entityType::eT_Ship)
    {
        this->SoundSrc->play3d(false,true);
    }
 }


asteroidEnt::~asteroidEnt()
{
}
