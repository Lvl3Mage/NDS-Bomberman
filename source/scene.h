#ifndef Scene_H
#define Scene_H

class Scene;

#include <vector>
#include <memory>
#include "config.h"
#include "terrain.h"
#include "cameracontroller.h"
#include "player.h"
#include "vector2.h"
using namespace std;
class Scene
{
public:
	Scene(u16* associatedMemory);
	vector<shared_ptr<Player>> players;
	unique_ptr<CameraController> cameraController;
	unique_ptr<Terrain> terrain;
	float sceneTime;
	int activePlayerIndex = 0;

	void UpdateTime(float newTime);
	void Update();

	Vector2 LoopCoord(Vector2 coord);
};
#endif