
#include <stdio.h>
#include "cameracontroller.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include "vector2.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;


CameraController::CameraController(u16* attachedMapMemory, int posX, int posY){
	screenMemory = attachedMapMemory;
	position = Vector2(posX, posY);
}
int CameraController::ScreenCoordToIndex(int x, int y){
	return (camHeight - 1 - y) * camWidth + x;
}
bool CameraController::IsCoordInFrame(Vector2 coord){
	return (coord.x >= 0) && (coord.x < camWidth) && (coord.y >= 0) && (coord.y < camHeight);
}
void CameraController::Render(Scene* scene){
	Vector2 screenPos = GetCornerPosition();
	//World Pass
	for (int screenY = 0; screenY < camHeight; screenY++)
	{
		for (int screenX = 0; screenX < camWidth; screenX++)
		{
			int worldCoordX = screenX+screenPos.x;
			int worldCoordY = screenY+screenPos.y;
			int pos_mapMemory = ScreenCoordToIndex(screenX,screenY); //y coord inverted
			screenMemory[pos_mapMemory] = scene->terrain->GetTerrainAt(worldCoordX,worldCoordY); // world sampling inverted
		}
	}

	//Player pass
	vector<shared_ptr<Player>> players = scene->players;
	for(int i = 0; i < players.size(); i++){
		int screenCoordX = players[i]->position.x - screenPos.x;
		int screenCoordY = players[i]->position.y - screenPos.y;
		if(IsCoordInFrame(Vector2(screenCoordX, screenCoordY))){
			int pos_mapMemory = ScreenCoordToIndex(screenCoordX,screenCoordY);
			screenMemory[pos_mapMemory] = players[i]->activeTile;
		}
	}
}
void CameraController::ClampPosition(){
	position.x = clamp(position.x, camWidth/2, W_SIZE - camWidth/2);
	position.y = clamp(position.y, camHeight/2, W_SIZE - camHeight/2);
}
Vector2 CameraController::GetCornerPosition(){
	return Vector2(position.x - camWidth/2, position.y - camHeight/2);
}
void CameraController::UpdateTarget(Scene* scene){
	lerpStartTime = scene->sceneTime;
	lerpEndTime = scene->sceneTime + camLerpTime;
	targetPlayerIndex = scene->activePlayerIndex;
	lerpStart.x = position.x;
	lerpStart.y = position.y;
}
void CameraController::SetPosition(Vector2 newPos){
	position = newPos;
	ClampPosition();
}
void CameraController::Update(Scene* scene){

	if(targetPlayerIndex != scene->activePlayerIndex){
		UpdateTarget(scene);
	}

	shared_ptr<Player> targetPlayer = scene->players[targetPlayerIndex];
	if(lerpEndTime>scene->sceneTime){
		float t = (scene->sceneTime - lerpStartTime)/(lerpEndTime - lerpStartTime);

		const float slope = 1.8;
		const float tPow = pow(t,slope);
		t = tPow / (tPow + pow(1-t,slope));
		// t = t/(1*(1-t)+1);
		int posX = round(lerp(lerpStart.x, targetPlayer->position.x, t));
		int posY = round(lerp(lerpStart.y, targetPlayer->position.y, t));
		SetPosition(Vector2(posX,posY));
		return;
	}
	SetPosition(Vector2(targetPlayer->position.x,targetPlayer->position.y));
}