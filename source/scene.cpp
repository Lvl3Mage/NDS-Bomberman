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

Explosion::Explosion(Vector2 pos, int rad){
	position = pos;
	radius = rad;
	tileOffset = 7;
}
template<int TerrainSize>
void GenerateTerrainData(float dataTarget[TerrainSize+1][TerrainSize+1], int seed){
	for(int i=0; i<TerrainSize+1;i++){
		for(int j=0; j<TerrainSize+1;j++){
			float weight = (ValueNoise(12,i+seed,j+seed,W_SIZE+1,0,100)/100); // 0-1
			dataTarget[i][j] = weight; // 0 - 1
		}
	}
}

void Scene::UpdateExplosionData(){
	for(int i=0; i<explosions.size();i++){
		explosions[i]->tileOffset -=1;
		if(explosions[i]->tileOffset == 0){
			explosions.erase(explosions.begin()+i);
		}
	}
}
Scene::Scene(u16* associatedMemory, int seed){

	float terrainData[W_SIZE+1][W_SIZE+1] = {0};
	GenerateTerrainData<W_SIZE>(terrainData, seed);

	terrain = make_unique<Terrain>(terrainData, 0.5, 0.8, 0, 1);
	terrain->MarchTerrainData();

	for(int i = 0; i < 2; i++){
		while(true){
			Vector2 coord = Vector2(RandomRange(0,W_SIZE),RandomRange(0,W_SIZE));
			if(!terrain->IsTerrainAt(coord)){
				players.push_back(make_shared<Player>(coord.x,coord.y));
				break;
			} 
		}
	}

	cameraController = make_unique<CameraController>(associatedMemory,  W_SIZE/2,W_SIZE/2);

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
void Scene::LogEndgame(){
	vector<int> alivePlayers = GetAlivePlayerIndecies();
	if(alivePlayers.size() == 0){
		iprintf("\x1b[2;2H Draw!" );
		iprintf("\x1b[16;2H Press X to restart");
		return;
	}

	iprintf("\x1b[2;2H Player %d won!", alivePlayers[0]+1);
	iprintf("\x1b[22;2H Press X to restart");
}
void Scene::LogSceneInfo(){
	iprintf("\x1b[2;2H Player %d's turn!", activePlayerIndex+1);
	iprintf("\x1b[4;2H Time left: %d seconds", (int)ceil(turnTimeLeft));
	iprintf("\x1b[6;2H Remaining movement: %d cells", players[activePlayerIndex]->remainingMovement);
	iprintf("\x1b[8;2H Remaining actions: %d", players[activePlayerIndex]->remainingActions);
	iprintf("\x1b[10;2H Selected action: %s", players[activePlayerIndex]->GetSelectedActionName());
	iprintf("\x1b[12;2H Health remaining: %d", players[activePlayerIndex]->health);


	iprintf("\x1b[18;2H Controlls:");

	iprintf("\x1b[20;2H L - Grenades, R - Walls");
	iprintf("\x1b[22;2H A - Move, X - Skip turn");
}
vector<int> Scene::GetAlivePlayerIndecies(){
	vector<int> alivePlayers;
	for(int i = 0; i < players.size(); i++){
		if(players[i]->health > 0){
			alivePlayers.push_back(i);
		}
	}
	return alivePlayers;
}
int Scene::AlivePlayerCount(){
	int count = 0;
	for(int i = 0; i < players.size(); i++){
		if(players[i]->health > 0){
			count++;
		}
	}
	return count;

}
void Scene::Update(){
	deltaTime = sceneTime - lastFrameTime;


	turnTimeLeft -= deltaTime;

	if(lastExplosionUpdateTime + explosionAnimTime < sceneTime){
		lastExplosionUpdateTime = sceneTime;
		UpdateExplosionData();
	}



	if(AlivePlayerCount() > 1){

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
	}
	else{
		LogEndgame();
		vector<int> alivePlayers = GetAlivePlayerIndecies();
		if(alivePlayers.size() > 0){
			activePlayerIndex = alivePlayers[0];
		}
	}


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
float Scene::ClosestPlayerDistance(Vector2 coord){
	float minDistance = W_SIZE*W_SIZE;
	for(shared_ptr<Player> player : players){
		Vector2 delta = Vector2(player->position.x - coord.x, player->position.y - coord.y);
		float distance = sqrt(delta.x*delta.x + delta.y*delta.y);
		if(minDistance > distance){
			minDistance = distance;
		}
	}
	return minDistance;
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
void Scene::AddExplosion(Vector2 position, int radius){
	explosions.push_back(make_shared<Explosion>(position, radius));
}