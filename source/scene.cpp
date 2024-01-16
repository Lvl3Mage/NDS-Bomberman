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

	players.push_back(make_shared<Player>(128,128));

	players.push_back(make_shared<Player>(40,40));
	//add players

	cameraController = make_unique<CameraController>(associatedMemory, 0,0);
}

void Scene::UpdateTime(float newTime){
	sceneTime = newTime;
}

void Scene::Update(){
	players[activePlayerIndex]->UpdateMovement(this);

	for(int i = 0; i < players.size(); i++){
		players[i]->Update(this);
	}

	activePlayerIndex = 1;//(int)round(sceneTime/5) % 2;



	cameraController->Update(this);
	cameraController->Render(this);

}

Vector2 Scene::LoopCoord(Vector2 coord){
	if(coord.x < 0 ){
		coord.x = W_SIZE-1;
	}
	if(coord.x > W_SIZE-1 ){
		coord.x = 0;
	}
	if(coord.y < 0 ){
		coord.y = W_SIZE-1;
	}
	if(coord.y > W_SIZE-1 ){
		coord.y = 0;
	}
	return coord;
}