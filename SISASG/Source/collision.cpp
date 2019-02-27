#include "collision.h"
#include <cmath>
#include <limits>
#include <unordered_set>
#include <algorithm>
#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // max

struct ChunkPos
{
    int Chnkx;
    int Chnky;
};

bool Separated(entity *Ent, entity *Ent2, Vector3 Axis, bool printseperations = false)
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
    if (printseperations)
    {
        std::cout << longSpan - sumSpan << "|" << (longSpan >= sumSpan) << std::endl;
    }
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
    chnk.Chnkx = (int)ceil(pos.x / 10);
    chnk.Chnky = (int)ceil(pos.y / 10);
    return chnk;
}

bool Intersects(entity *Ent1, entity *Ent2)
{
    if (Separated(Ent1, Ent2, Ent1->right) || Separated(Ent1, Ent2, Ent1->up) ||
        Separated(Ent1, Ent2, Ent1->view) || Separated(Ent1, Ent2, Ent2->right) ||
        Separated(Ent1, Ent2, Ent2->up) || Separated(Ent1, Ent2, Ent2->view) ||
        Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->right)) ||
        Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->up)) ||
        Separated(Ent1, Ent2, Ent1->right.Cross(Ent2->view)) ||
        Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->right)) ||
        Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->up)) ||
        Separated(Ent1, Ent2, Ent1->up.Cross(Ent2->view)) ||
        Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->right)) ||
        Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->up)) ||
        Separated(Ent1, Ent2, Ent1->view.Cross(Ent2->view))
        )
    {
        return false;
    }
    else
    {
        return true;
    }
    
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

void collision::doCollisions(std::map<std::string, entity*> &entityList, double dt)
{
    entity* Ent2;
    entity* Ent;
    std::map<std::string, entity*>::iterator it;
    std::map<std::string, entity*>::iterator it2;
    for (it = entityList.begin(); it != entityList.end(); it++)
    {
        Ent = it->second;
        if (Ent->physics)
        {
            Ent->UpdateBBox();
        }
        
    }
    for (it = entityList.begin(); it != entityList.end(); it++)
    {
        Ent = it->second;
        if (Ent->physics)
        {
            this->updatingEnts += 1;
            if (Ent->velocity != .0f)
            {
                for (it2 = entityList.begin(); it2 != entityList.end(); it2++)
                {
                    Ent2 = it2->second;
                    if (Ent != Ent2)
                    {
                        
                        if (Ent2->physics)
                        {
                            Ent2->UpdateBBox();
                            entity *EntNext = new entity;
                            entity *Ent2Next = new entity;
                            *EntNext = *Ent;
                            *Ent2Next = *Ent2;

                            EntNext->position = Ent->position + (dt * Ent->velocity) * Ent->view;
                            Ent2Next->position = Ent2->position + (dt * Ent2->velocity) * Ent2->view;
                            this->updatingEnts += 1;
                            if (Intersects(Ent, Ent2))
                            {
                                float velocity = Ent->velocity + Ent2->velocity;
                                if (Ent->velocity > Ent2->velocity)
                                {
                                    if (Ent->type != entityType::eT_Ship)
                                    {
                                        Ent->velocity = Ent->velocity + velocity / 4;
                                    }
                                    if (Ent2->type != entityType::eT_Ship)
                                    {
                                        Ent2->velocity = velocity / 2;
                                    }
                                    if (Ent2->type != entityType::eT_Ship)
                                    {
                                        Ent2->view = Ent->view;
                                    }
                                    if (Ent2->type == entityType::eT_Bullet && Ent->type == entityType::eT_Object)
                                    {
                                        Ent->doDestroy = true;
                                        Ent2->doDestroy = true;
                                    }
                                    else if (Ent2->type == entityType::eT_Object && Ent->type == entityType::eT_Bullet)
                                    {
                                        Ent->doDestroy = true;
                                        Ent2->doDestroy = true;
                                    }
                                    
                                    Ent2->OnHit(Ent);
                                }
                                else
                                {
                                    if (Ent2->type == entityType::eT_Bullet && Ent2->type == Ent->type)
                                    if (Ent2->type != entityType::eT_Ship)
                                    {
                                        Ent2->velocity = 50.0f;
                                    }
                                    if (Ent->type != entityType::eT_Ship)
                                    {
                                        Ent->velocity = Ent->velocity + velocity / 2;
                                    }
                                    if (Ent2->type != entityType::eT_Ship)
                                    {
                                        Ent2->view = Ent->view;
                                    }
                                    if (Ent2->type == entityType::eT_Bullet && Ent->type == entityType::eT_Object)
                                    {
                                        Ent->doDestroy = true;
                                        Ent2->doDestroy = true;
                                    }
                                    else if (Ent2->type == entityType::eT_Object && Ent->type == entityType::eT_Bullet)
                                    {
                                        Ent->doDestroy = true;
                                        Ent2->doDestroy = true;
                                    }
                                    Ent->OnHit(Ent2);
                                }
                            }
                            // https://www.youtube.com/watch?v=fTVB2Sxa2Cs delete
                            delete EntNext;
                            delete Ent2Next;
                        }
                    }
                    
                }
            }
        }
    }
}