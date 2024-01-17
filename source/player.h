#ifndef Player_H
#define Player_H
class Player;


#include <string>
#include "scene.h"
#include "config.h"
#include "terrain.h"
#include "vector2.h"
using namespace std;
class Player
{
	int selectedActionType = 0;
	//0 -> movement
	//1 -> grenades

	void ProcessAttack(Scene* scene);
	void ProcessMovement(Scene* scene);
public:
	int health = 3;
	int remainingActions;
	int remainingMovement;
	Vector2 position = Vector2(0,0);
	u8 activeTile;


	float lastMoveTime = 0;
	const float moveCooldown = 0.1; 
	Vector2 moveDelta = Vector2(0,0);
	
	Player(int startX, int startY);
	void PassiveUpdate(Scene* scene);
	void ActiveUpdate(Scene* scene);
	void ResetTurn();
	char* GetSelectedActionName();
	void Damage(int damage);
	void OnDeath();
};
#endif