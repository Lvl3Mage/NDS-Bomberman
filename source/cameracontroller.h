#include <nds.h>
#include <vector>
#include "config.h"

class CameraController
{
	u16* mapMemory;
	int camX=0;
	int camY=0;
	const int camWidth = 32;
	const int camHeight = 24;
public:
	CameraController(u16* attachedMapMemory)
	void RenderCamera();
	void MoveCamera(int targetX, int targetY);
	void InterpolateCamera(int targetX, int targetY);
};