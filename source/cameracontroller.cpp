#include "cameracontroller.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;
CameraController::CameraController(u16* attachedMapMemory, int posX, int posY){
	screenMemory = attachedMapMemory;
	camX = posX;
	camY = posY;
}
int CameraController::ScreenCoordToIndex(int x, int y){
	return (camHeight - 1 - y) * camWidth + x;
}
void CameraController::Render(Scene* scene){
	//World Pass
	for (int screenY = 0; screenY < camHeight; screenY++)
	{
		for (int screenX = 0; screenX < camWidth; screenX++)
		{
			int worldCoordX = screenX+camX;
			int worldCoordY = screenY+camY;
			int pos_mapMemory = ScreenCoordToIndex(screenX,screenY); //y coord inverted
			screenMemory[pos_mapMemory] = scene->terrain->GetTerrainAt(worldCoordX,worldCoordY); // world sampling inverted
		}
	}

	//Player pass
	vector<shared_ptr<Player>> players = scene->players;
	for(int i = 0; i < players.size(); i++){
		int screenCoordX = players[i]->x-camX;
		int screenCoordY = players[i]->y-camY;
		int pos_mapMemory = ScreenCoordToIndex(screenCoordX,screenCoordY);
		screenMemory[pos_mapMemory] = players[i]->activeTile;
	}
}
void CameraController::ClampPosition(){
	camX = clamp(camX, 0, W_SIZE - camWidth);
	camY = clamp(camY, 0, W_SIZE - camHeight);
}
void CameraController::SetPosition(int targetX, int targetY){
	camX = targetX - camWidth/2;
	camY = targetY - camHeight/2;
	ClampPosition();
}
void CameraController::UpdateTarget(Scene* scene){
	lerpStartTime = scene->sceneTime;
	lerpEndTime = scene->sceneTime + camLerpTime;
	targetPlayerIndex = scene->activePlayerIndex;
}
void CameraController::Update(Scene* scene){

	if(targetPlayerIndex != scene->activePlayerIndex){
		UpdateTarget(scene);
	}


	shared_ptr<Player> targetPlayer = scene->players[targetPlayerIndex];
	if(lerpEndTime>scene->sceneTime){
		float t = (scene->sceneTime - lerpStartTime)/(lerpEndTime-lerpStartTime);

		t = t/(1*(1-t)+1);
		int posX = round(lerp(lerpStartX, targetPlayer->x, t));
		int posY = round(lerp(lerpStartY, targetPlayer->y, t));
		SetPosition(posX,posY);
		return;
	}
	SetPosition(targetPlayer->x,targetPlayer->y);
}