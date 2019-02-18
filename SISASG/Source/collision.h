#pragma once
#include "entity.h"
#include <vector>
#include <map>

class Chunk
{
    std::vector<entity*> ChunkEntities;
    int x;
    int y;
public:
    Chunk(int x, int y) { this->x = x; this->y = y; };
    Chunk() { this->x = 0; this->y = 0; };
    void addEnt(entity* ent) { this->ChunkEntities.push_back(ent); }
    void popEnt(entity* ent);
};

class collision
{
    std::vector<entity*> *entityList;
    std::map<long int,Chunk> *chunks;
    std::map<long int,Chunk>::iterator chunkit;
public:
	collision();
	collision(std::vector<entity*> *entityList);
    ~collision();
    Chunk* findChunkFast(entity* entity);
};

