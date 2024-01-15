#include "scene.h"
#include "terrain.h"
#include "player.h"
#include "config.h"
#include "utils.h"
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

using namespace std;


template<int TerrainSize>
void GenerateTerrainData(int dataTarget[TerrainSize+1][TerrainSize+1]){
	for(int i=0; i<TerrainSize+1;i++){
		for(int j=0; j<TerrainSize+1;j++){
			dataTarget[i][j] = ValueNoise(12,i,j,W_SIZE+1,0,100);
		}
	}
}


Scene::Scene(u16* associatedMemory){

	int terrainData[W_SIZE+1][W_SIZE+1] = {0};
	GenerateTerrainData<W_SIZE>(terrainData);

	terrain = make_unique<Terrain>(terrainData);
	terrain->MarchTerrainData(50);

	players.push_back(make_shared<Player>(0,0));
	//add players

	cameraController = make_unique<CameraController>(associatedMemory, 0,0);
}

void Scene::UpdateTime(float newTime){
	sceneTime = newTime;
}

void Scene::Update(){
	cameraController->Update(this);
	cameraController->Render(this);

}