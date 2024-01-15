#ifndef Scene_H
#define Scene_H

class Scene;

#include <vector>
#include <memory>
#include "config.h"
#include "terrain.h"
#include "cameracontroller.h"
#include "player.h"
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
};
#endif