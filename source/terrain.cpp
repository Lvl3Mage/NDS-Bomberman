#include "terrain.h"
#include "config.h"
#include "vector2.h"
#include "tileconfig.h"
#include "scene.h"
#include <algorithm>
#include <cmath>

using namespace std;
template<int TerrainSize>
uint GetSquareConfig(float data[TerrainSize+1][TerrainSize+1], int x, int y, float surfaceLevel, float heavyLevel){
	uint squareConfig = 0;
	float totalWeight = 0;
	for(int yOffset = 0; yOffset <= 1; yOffset++){
		for(int xOffset = 0; xOffset <= 1; xOffset++){
			totalWeight += data[x + xOffset][y + yOffset];
			squareConfig = squareConfig << 1;
			if(data[x + xOffset][y + yOffset] > surfaceLevel){
				squareConfig++;
			}
		}
	}
	totalWeight /= 4;
	if(totalWeight > heavyLevel){
		squareConfig += terrainTiles2;
	}

	return squareConfig;
}
template<int TerrainSize>
void SquareMarchRegion(Vector2 start, Vector2 end, float data[TerrainSize+1][TerrainSize+1], int target[TerrainSize][TerrainSize], float surfaceLevel, float heavyLevel){
	start.x = clamp(start.x-1,0,TerrainSize);
	start.y = clamp(start.y-1,0,TerrainSize);
	end.x = clamp(end.x+1,0,TerrainSize);
	end.y = clamp(end.y+1,0,TerrainSize);
	for(int i=start.x-1; i<end.x;i++){
		for(int j=start.y-1; j<end.y;j++){
			uint tileIndex = GetSquareConfig<TerrainSize>(data, i, j, surfaceLevel,heavyLevel);
			target[i][j] = tileIndex;
		}
	}
}
template<int TerrainSize>
void SquareMarch(float data[TerrainSize+1][TerrainSize+1], int target[TerrainSize][TerrainSize], float surfaceLevel, float heavyLevel){
	SquareMarchRegion<TerrainSize>(Vector2(0,0), Vector2(TerrainSize, TerrainSize), data,target, surfaceLevel,heavyLevel);
}

Terrain::Terrain(float data[W_SIZE+1][W_SIZE+1], float _surfaceLevel, float _heavyLevel, float _minTerrain, float _maxTerrain){
	for(int i=0; i<W_SIZE+1;i++){
		for(int j=0; j<W_SIZE+1;j++){
			terrainData[i][j] = data[i][j];
		}
	}
	surfaceLevel = _surfaceLevel;
	heavyLevel = _heavyLevel;
	minTerrain = _minTerrain;
	maxTerrain = _maxTerrain;
}
int Terrain::GetTerrainAt(Vector2 coord){
	return terrainTiles[coord.x][coord.y];
}

bool Terrain::IsTerrainAt(Vector2 coord){
	return terrainTiles[coord.x][coord.y] != terrainTiles1 && terrainTiles[coord.x][coord.y] != terrainTiles2;
}
void Terrain::MarchTerrainData(){
	SquareMarch<W_SIZE>(terrainData, terrainTiles, surfaceLevel,heavyLevel);
}
void Terrain::MarchTerrainDataRegion(Vector2 start, Vector2 end){
	SquareMarchRegion<W_SIZE>(start, end, terrainData, terrainTiles, surfaceLevel,heavyLevel);
}
void Terrain::TerraformCircle(Scene* scene, Vector2 center, int radius, float amount ){
	int startX = center.x - radius;
	startX = clamp(startX, 0, W_SIZE+1);
	int endX = center.x + radius;
	endX = clamp(endX, 0, W_SIZE+1);
	int startY = center.y - radius;
	startY = clamp(startY, 0, W_SIZE+1);
	int endY = center.y + radius;
	endY = clamp(endY, 0, W_SIZE+1);

	for(int i=startX; i<endX;i++){
		for(int j=startY; j<=endY;j++){
			if(scene->ClosestPlayerDistance(Vector2(i,j)) < 2){//skip square if too close to player
				continue;
			}

			Vector2 delta = Vector2(i - center.x, j - center.y);
			float distance = sqrt(delta.x*delta.x + delta.y*delta.y);
			if(distance <= radius){
				terrainData[i][j] += amount;
				terrainData[i][j] = clamp(terrainData[i][j],minTerrain,maxTerrain);
			}
		}
	}
	MarchTerrainDataRegion(Vector2(startX,startY), Vector2(endX, endY));
}
