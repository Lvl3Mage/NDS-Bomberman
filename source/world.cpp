#include "world.h"
#include "config.h"
template<int WorldSize>
uint GetSquareConfig(int data[WorldSize+1][WorldSize+1], int x, int y, int surfaceLevel){
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
template<int WorldSize>
void SquareMarch(int data[WorldSize+1][WorldSize+1], int target[WorldSize][WorldSize], int tileIndexStart, int surfaceLevel){
	for(int i=0; i<WorldSize;i++){
		for(int j=0; j<WorldSize;j++){
			uint tileIndex = tileIndexStart + GetSquareConfig<WorldSize>(data, i, j, surfaceLevel);
			target[i][j] = tileIndex;
		}
	}
}


World::World(int data[W_SIZE+1][W_SIZE+1]){
	for(int i=0; i<W_SIZE+1;i++){
		for(int j=0; j<W_SIZE+1;j++){
			worldData[i][j] = data[i][j];
		}
	}
}
int World::GetWorldAt(int x, int y){
	return worldTiles[x][y];
}
void World::MarchWorldData(int surfaceLevel, int startTileIndex){
	SquareMarch<W_SIZE>(worldData, worldTiles, startTileIndex, surfaceLevel);
}
