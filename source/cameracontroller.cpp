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
void CameraController::Render(Scene* scene){
	for (int screenY = 0; screenY < camHeight; screenY++)
	{
		for (int screenX = 0; screenX < camWidth; screenX++)
		{
			int worldCoordX = screenX+camX;
			int worldCoordY = screenY+camY;
			int pos_mapMemory = (camHeight-1-screenY) * camWidth + screenX; //y coord inverted
			screenMemory[pos_mapMemory] = scene->terrain->GetTerrainAt(worldCoordX,W_SIZE-1-worldCoordY); // world sampling inverted
		}
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