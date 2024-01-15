#include "terrain.h"
#include "config.h"


template<int TerrainSize>
uint GetSquareConfig(int data[TerrainSize+1][TerrainSize+1], int x, int y, int surfaceLevel){
	uint squareConfig = 0;
	for(int yOffset = 0; yOffset <= 1; yOffset++){
		for(int xOffset = 0; xOffset <= 1; xOffset++){
			squareConfig = squareConfig << 1;
			if(data[x + xOffset][y + yOffset] > surfaceLevel){
				squareConfig++;
			}
		}
	}
	return squareConfig;
}
template<int TerrainSize>
void SquareMarch(int data[TerrainSize+1][TerrainSize+1], int target[TerrainSize][TerrainSize], int surfaceLevel){
	for(int i=0; i<TerrainSize;i++){
		for(int j=0; j<TerrainSize;j++){
			uint tileIndex = GetSquareConfig<TerrainSize>(data, i, j, surfaceLevel);
			target[i][j] = 0;
		}
	}
}


Terrain::Terrain(int data[W_SIZE+1][W_SIZE+1]){
	for(int i=0; i<W_SIZE+1;i++){
		for(int j=0; j<W_SIZE+1;j++){
			terrainData[i][j] = data[i][j];
		}
	}
}
int Terrain::GetTerrainAt(int x, int y){
	return terrainTiles[x][y];
}
void Terrain::MarchTerrainData(int surfaceLevel){
	SquareMarch<W_SIZE>(terrainData, terrainTiles, surfaceLevel);
}
