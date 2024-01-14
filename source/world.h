#ifndef SOURCE_WORLD_H
#define SOURCE_WORLD_H
#include <nds.h>
#include <vector>
#include "config.h"

class World
{
    int worldData[W_SIZE+1][W_SIZE+1] = {0};
    int worldTiles[W_SIZE][W_SIZE];

public:
    World(int data[W_SIZE+1][W_SIZE+1]);
    void MarchWorldData(int surfaceLevel, int startTileIndex);
    int GetWorldAt(int x, int y);
};
#endif