#ifndef Terrain_H
#define Terrain_H
#include <nds.h>
#include <vector>
#include "config.h"
#include "vector2.h"

class Terrain
{
    int terrainData[W_SIZE+1][W_SIZE+1] = {0};
    int terrainTiles[W_SIZE][W_SIZE];

public:
    Terrain(int data[W_SIZE+1][W_SIZE+1]);
    void MarchTerrainData(int surfaceLevel);
    int GetTerrainAt(Vector2 coord);
    bool IsTerrainAt(Vector2 coord);
};
#endif