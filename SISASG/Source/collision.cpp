#include "collision.h"
#include <cmath>
#include <limits>
#include <unordered_set>
#include <algorithm>
struct ChunkPos
{
    int Chnkx;
    int Chnky;
};

bool Separated(entity *Ent, entity *Ent2, Vector3 Axis)
{
    Vector3 Ent1V[] = {
    Ent->HBox.frontLeftUp,
    Ent->HBox.frontLeftDown,
    Ent->HBox.frontRightUp,
    Ent->HBox.frontRightDown,

    Ent->HBox.backLeftUp,
    Ent->HBox.backLeftDown,
    Ent->HBox.backRightUp,
    Ent->HBox.backLeftDown };

    Vector3 Ent2V[] = {
    Ent2->HBox.frontLeftUp,
    Ent2->HBox.frontLeftDown,
    Ent2->HBox.frontRightUp,
    Ent2->HBox.frontRightDown,

    Ent2->HBox.backLeftUp,
    Ent2->HBox.backLeftDown,
    Ent2->HBox.backRightUp,
    Ent2->HBox.backLeftDown };

    float aMin = std::numeric_limits<float>::max();
    float aMax = std::numeric_limits<float>::min();
    float bMin = std::numeric_limits<float>::max();
    float bMax = std::numeric_limits<float>::min();

    for (size_t i = 0; i < 8; i++)
    {
        float aDist = Ent1V[i].Dot(Axis);
        aMin = aDist < aMin ? aDist : aMin;
        aMax = aDist > aMax ? aDist : aMax;
        float bDist = Ent2V[i].Dot(Axis);
        bMin = bDist < bMin ? bDist : bMin;
        bMax = bDist > bMax ? bDist : bMax;
    }
    float longSpan = std::fmaxf(aMax, bMax) - std::fminf(aMin, bMin);
    float sumSpan = aMax - aMin + bMax - bMin;
    return longSpan >= sumSpan;
}

long int getChunkHash(int x, int y)
{
    long int m = (long int)powl(2,32) * (2654435761);
    return x * m + y * m;
}

ChunkPos getChunk(Vector3 pos)
{
    ChunkPos chnk;
    chnk.Chnkx = ceil(pos.x / 10);
    chnk.Chnky = ceil(pos.y / 10);
    return chnk;
}

void collision::doCollisions(std::vector<entity*> &entityList)
{
    for (size_t i = 0; i < (entityList).size(); i++)
    {
        entity* Ent = (entityList)[i];
        if (Ent->physics)
        {
            Ent->UpdateBBox();
            this->updatingEnts += 1;
        }
    }
}

bool Intersects(entity *Ent1, entity *Ent2)
{
    if (Separated(Ent1, Ent2, Ent1->right))
        return false;
    if (Separated(Ent1, Ent2, Ent1->up))
        return false;
    if (Separated(Ent1, Ent2, Ent1->view))
        return false;

    if (Separated(Ent1, Ent2, Ent2->right))
        return false;
    if (Separated(Ent1, Ent2, Ent2->up))
        return false;
    if (Separated(Ent1, Ent2, Ent2->view))
        return false;

    if (Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->right)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->up)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->view)))
        return false;

    if (Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->right)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->up)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->view)))
        return false;

    if (Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->right)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->up)))
        return false;
    if (Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->view)))
        return false;
}

void Chunk::popEnt(entity * ent)
{
    for (size_t i = 0; i < this->ChunkEntities.size(); i++)
    {
        if (this->ChunkEntities[i] == ent)
        {
            this->ChunkEntities.erase(this->ChunkEntities.begin() + i);
        }
    }
}
