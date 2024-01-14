#include <nds.h>
#include <vector>
#include "config.h"
#include "world.h"
#include <memory>

using namespace std;

class CameraController
{
	u16* mapMemory;
	int camX=0;
	int camY=0;
	const int camWidth = 32;
	const int camHeight = 24;
	void ClampPosition();
public:
	CameraController(u16* attachedMapMemory);
	void RenderCamera(unique_ptr<World> world);
	void MoveCamera(int targetX, int targetY);
	void InterpolateCamera(int targetX, int targetY);
};