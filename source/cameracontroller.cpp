#include "cameracontroller.h"
#include "world.h"
#include "config.h"
#include "utils.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;
CameraController::CameraController(u16* attachedMapMemory){

}
void CameraController::RenderCamera(unique_ptr<World> world){
	for (int screenY = 0; screenY < camHeight; screenY++)
	{
		for (int screenX = 0; screenX < camWidth; screenX++)
		{
			int worldCoordX = screenX+camX;
			int worldCoordY = screenY+camY;
			int pos_mapMemory = (camHeight-1-screenY) * camWidth + screenX; //y coord inverted
			mapMemory[pos_mapMemory] = world->GetWorldAt(worldCoordX,W_SIZE-1-worldCoordY); // world sampling inverted
		}
	}
}
void CameraController::ClampPosition(){
	camX = clamp(camX, 0, W_SIZE - camWidth);
	camY = clamp(camY, 0, W_SIZE - camHeight);
}
void CameraController::MoveCamera(int targetX, int targetY){
	camX = targetX - camWidth/2;
	camY = targetY - camHeight/2;
	ClampPosition();
}
void CameraController::InterpolateCamera(int targetX, int targetY){

	ClampPosition();
}