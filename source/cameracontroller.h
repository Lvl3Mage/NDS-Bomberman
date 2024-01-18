#ifndef CamController_H
#define CamController_H

class CameraController;

#include <nds.h>
#include <vector>
#include <memory>
#include "scene.h"
#include "vector2.h"
#include "config.h"
#include "terrain.h"

using namespace std;

class CameraController
{
	u16* screenMemory;
	Vector2 position = Vector2(0,0);
	const int camWidth = 32;
	const int camHeight = 24;
	const float camLerpTime = 1.5;

	Vector2 lerpStart = Vector2(0,0);
	int lerpStartX, lerpStartY;
	float lerpStartTime;
	float lerpEndTime;
	void ClampPosition();
	Vector2 GetCornerPosition();
	bool IsCoordInFrame(Vector2 coord);
	bool IsCoordInFrame(Vector2 coord, int radius);
	int targetPlayerIndex = 1;
	void UpdateTarget(Scene* scene);
	int ScreenCoordToIndex(int x, int y);
	void SetPosition(Vector2 newPos);

	void DrawBG(Vector2 screenPos, Scene* scene);
	void DrawExplosions(Vector2 screenPos, Scene* scene);
	void DrawProjectiles(Vector2 screenPos, Scene* scene);
	void DrawWalls(Vector2 screenPos, Scene* scene);
	void DrawPlayers(Vector2 screenPos, Scene* scene);
public:

	CameraController(u16* attachedMapMemory, int posX, int posY);
	void Render(Scene* scene);
	void Update(Scene* scene);
};
#endif