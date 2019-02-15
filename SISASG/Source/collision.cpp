#include "collision.h"
#include <cmath>
struct ChunkPos
{
    int Chnkx;
    int Chnky;
};

long int getChunkHash(int x, int y)
{
    long int m = powl(2,32) * (2654435761);
    return x * m + y * m;
}

ChunkPos getChunk(Vector3 pos)
{
    ChunkPos chnk;
    chnk.Chnkx = ceil(pos.x / 10);
    chnk.Chnky = ceil(pos.y / 10);
    return chnk;
}

collision::collision()
{
    
}

collision::collision(std::vector<entity*>* entityList)
{
    for (size_t i = 0; i < this->entityList[i].size(); i++)
    {
        entity* Ent = this->entityList->operator[][i];
        // if its almost not moving, it's not moving *Dabs*
        // Save some time with simulation
        if (roundf(Ent->velocity*1000)/1000 != 0.0f) 
        {
            // we update the BBox.
            Ent->UpdateBBox();
            this->findChunkFast(Ent);
        }
    }
}

Chunk collision::findChunkFast(entity* ent)
{
    ChunkPos pos = getChunk(ent->position);
    long int CHash = getChunkHash(pos.Chnkx, pos.Chnky);
    if (this->chunks->find(getChunkHash(pos.Chnkx, pos.Chnky)) == this->chunks->end())
    {
        return this->chunks->find(CHash)->second;
    }
    else
    {
        Chunk nchunk;
        nchunk = Chunk(pos.Chnkx, pos.Chnky);
        nchunk.
        std::pair<long int, Chunk> v(CHash, nchunk);
        this->chunks->insert(v);

    }
    
}

collision::~collision()
{

}