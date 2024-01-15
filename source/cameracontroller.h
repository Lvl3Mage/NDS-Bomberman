#ifndef CamController_H
#define CamController_H

class CameraController;

#include <nds.h>
#include <vector>
#include <memory>
#include "scene.h"
#include "config.h"
#include "terrain.h"

using namespace std;

class CameraController
{
	u16* screenMemory;
	int camX=0;
	int camY=0;
	const int camWidth = 32;
	const int camHeight = 24;
	const float camLerpTime = 1;

	int lerpStartX, lerpStartY;
	float lerpStartTime;
	float lerpEndTime;
	void ClampPosition();
	void SetPosition(int targetX, int targetY);
	int targetPlayerIndex;
	void UpdateTarget(Scene* scene);
public:

	CameraController(u16* attachedMapMemory, int posX, int posY);
	void Render(Scene* scene);
	void Update(Scene* scene);
};
#endif