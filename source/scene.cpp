#include <stdio.h>
#include "scene.h"
#include "terrain.h"
#include "player.h"
#include "config.h"
#include "utils.h"
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;


template<int TerrainSize>
void GenerateTerrainData(float dataTarget[TerrainSize+1][TerrainSize+1]){
	for(int i=0; i<TerrainSize+1;i++){
		for(int j=0; j<TerrainSize+1;j++){
			dataTarget[i][j] = ValueNoise(12,i,j,W_SIZE+1,0,100);
		}
	}
}


Scene::Scene(u16* associatedMemory){

	float terrainData[W_SIZE+1][W_SIZE+1] = {0};
	GenerateTerrainData<W_SIZE>(terrainData);

	terrain = make_unique<Terrain>(terrainData);
	terrain->MarchTerrainData(50);

	players.push_back(make_shared<Player>(198,198));

	players.push_back(make_shared<Player>(40,40));
	//add players

	cameraController = make_unique<CameraController>(associatedMemory,  128,128);

	NextTurn();
}

void Scene::UpdateTime(float newTime){
	sceneTime = newTime;
}
void Scene::NextTurn(){

	activePlayerIndex += 1;
	if(activePlayerIndex >= players.size()){
		activePlayerIndex = 0;
	}

	turnTimeLeft = 20;
	players[activePlayerIndex]->ResetTurn();
}
void Scene::LogSceneInfo(){
	iprintf("\x1b[2;2H Player %d's turn!", activePlayerIndex+1);
	iprintf("\x1b[4;2H Time left: %d seconds", (int)ceil(turnTimeLeft));
	iprintf("\x1b[6;2H Remaining movement: %d cells", players[activePlayerIndex]->remainingMovement);
	iprintf("\x1b[8;2H Remaining actions: %d", players[activePlayerIndex]->remainingActions);
	iprintf("\x1b[10;2H Selected action: %s", players[activePlayerIndex]->GetSelectedActionName());
}
void Scene::Update(){
	deltaTime = sceneTime - lastFrameTime;


	turnTimeLeft -= deltaTime;

	if(turnTimeLeft <= 0){
		NextTurn();
	}



	players[activePlayerIndex]->ActiveUpdate(this);

	for(int i = 0; i < players.size(); i++){
		players[i]->PassiveUpdate(this);
	}

	for(int i = 0; i < projectiles.size(); i++){
		projectiles[i]->Update(this);
	}


	LogSceneInfo();

	cameraController->Update(this);
	cameraController->Render(this);

	lastFrameTime = sceneTime;

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
bool Scene::IsPlayerCollision(Vector2 coord){
	for(shared_ptr<Player> player : players){
		if(player->position.x == coord.x && player->position.y == coord.y){
			return true;
		}
	}
	return false;
}
void Scene::AddProjectile(shared_ptr<Projectile> projectile){
	projectiles.push_back(projectile);
}
void Scene::RemoveProjectile(Projectile* projectile){//Not the best way to do this but it'll do 
	for(int i=0; i<projectiles.size(); i++){
		if(projectiles[i].get() == projectile){
			projectiles.erase(projectiles.begin()+i);
			return;
		}
	}
}