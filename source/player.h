#ifndef Player_H
#define Player_H
class Player;


#include "scene.h"
#include "config.h"
#include "terrain.h"
using namespace std;
class Player
{
	void ClampPosition();
public:
	int x=0;
	int y=0;
	u8 activeTile = 16;
	Player(int startX, int startY);
	void Update(Scene* scene);
};
#endif