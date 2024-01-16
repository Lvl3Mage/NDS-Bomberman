#ifndef Player_H
#define Player_H
class Player;


#include "scene.h"
#include "config.h"
#include "terrain.h"
#include "vector2.h"
using namespace std;
class Player
{
public:
	Vector2 position = Vector2(0,0);
	u8 activeTile = 16;

	float lastMoveTime = 0;
	const float moveCooldown = 0.1; 
	Vector2 moveDelta = Vector2(0,0);
	
	Player(int startX, int startY);
	void Update(Scene* scene);
	void UpdateMovement(Scene* scene);
};
#endif