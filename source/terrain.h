#ifndef Terrain_H
#define Terrain_H
class Terrain;
#include <nds.h>
#include <vector>
#include "config.h"
#include "vector2.h"
#include "scene.h"

class Terrain
{
    float terrainData[W_SIZE+1][W_SIZE+1] = {0};
    int terrainTiles[W_SIZE][W_SIZE];

public:
    float surfaceLevel;
    Terrain(float data[W_SIZE+1][W_SIZE+1], float _surfaceLevel);
    void MarchTerrainData();
    void MarchTerrainDataRegion(Vector2 start, Vector2 end);
    int GetTerrainAt(Vector2 coord);
    bool IsTerrainAt(Vector2 coord);
    void TerraformCircle(Scene* scene, Vector2 center, int radius, int amount );
};
#endif