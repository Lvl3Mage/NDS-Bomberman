#ifndef Terrain_H
#define Terrain_H
#include <nds.h>
#include <vector>
#include "config.h"
#include "vector2.h"

class Terrain
{
    float terrainData[W_SIZE+1][W_SIZE+1] = {0};
    int terrainTiles[W_SIZE][W_SIZE];

public:
    Terrain(float data[W_SIZE+1][W_SIZE+1]);
    void MarchTerrainData(float surfaceLevel);
    void MarchTerrainDataRegion(Vector2 start, Vector2 end, float surfaceLevel);
    int GetTerrainAt(Vector2 coord);
    bool IsTerrainAt(Vector2 coord);
    void TerraformCircle(Vector2 center, int radius, int amount );
};
#endif