#include "player.h"
#include "terrain.h"
#include "config.h"
#include "tileconfig.h"
#include "utils.h"
#include "scene.h"
#include "projectile.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;

Vector2 GetActionDir(u32 key){
	Vector2 actionDir = Vector2(0,0);
	if (key & KEY_RIGHT) {
		actionDir.x = 1;
	}
	if (key & KEY_LEFT){
		actionDir.x = -1;
	}
	
	if (key & KEY_UP) {
		actionDir.y = 1;
	}
	if (key & KEY_DOWN){
		actionDir.y = -1;
	}
	return actionDir;
}
Player::Player(int startX, int startY){
	position = Vector2(startX, startY);
	activeTile = playerTiles;
}
void Player::ActiveUpdate(Scene* scene){



	u32 key;
	key = keysDown();
	if(key & KEY_A){//movement
		selectedActionType = 0;
	}
	if(key & KEY_L){//grenades
		selectedActionType = 1;
	}
	if(key & KEY_R){//wall building
		selectedActionType = 2;
	}
	if(key & KEY_X){//skip turn
		scene->NextTurn();
	}


	switch(selectedActionType){
		case 0:
			ProcessMovement(scene);
			break;
		case 1:
		case 2:
			ProcessAttack(scene);
			break;
	}
}
void Player::ProcessAttack(Scene* scene){
	if(remainingActions <= 0){
		return;
	}
	u32 key;
	key = keysDown();
	Vector2 actionDir = GetActionDir(key);
	if(actionDir.x == 0 && actionDir.y == 0){
		return;	
	}

	remainingActions --;
	shared_ptr<Projectile> projectile;

	if(selectedActionType == 1){
		projectile = make_shared<Projectile>(Vector2(position.x+actionDir.x, position.y+actionDir.y), actionDir, 5, grenadeTiles, 0.3, 6, -0.25, 1, true);
	}
	else if(selectedActionType == 2){
		projectile = make_shared<Projectile>(Vector2(position.x+actionDir.x, position.y+actionDir.y), actionDir, 5, mudBallTiles, 0.1, 3, 1, 0, false);
	}

	scene->AddProjectile(projectile);

}
void Player::ProcessMovement(Scene* scene){

	u32 key;
	key = keysCurrent();
	moveDelta = GetActionDir(key);

	if(scene->sceneTime < lastMoveTime + moveCooldown){
		return;
	}
	if(remainingMovement <= 0){
		return;
	}
	if(moveDelta.x == 0 && moveDelta.y == 0){
		return;	
	}
	lastMoveTime = scene->sceneTime;
	remainingMovement--;

	Vector2 nextPos = Vector2(moveDelta.x + position.x, moveDelta.y + position.y);
	nextPos = scene->LoopCoord(nextPos);

	if(!scene->terrain->IsTerrainAt(Vector2(nextPos.x, position.y))){
		position.x = nextPos.x;
	}
	moveDelta.x = 0;

	if(!scene->terrain->IsTerrainAt(Vector2(position.x, nextPos.y))){
		position.y = nextPos.y;
	}
	moveDelta.y = 0;
}
void Player::PassiveUpdate(Scene* scene){
}
void Player::ResetTurn(){
	remainingActions = 3;
	remainingMovement = 50;
	selectedActionType = 0;
}
char* Player::GetSelectedActionName(){
	switch(selectedActionType){
		case 0:
			return "Move";
		case 1:
			return "Grenades";
		case 2:
			return "Wall building";
	}
	return "Other";
}
void Player::Damage(int damage){
	if(health<= 0){
		return;
	}
	health -= damage;
	health = max(health,0);
	if(health <= 0){
		OnDeath();
	}
}
void Player::OnDeath(){
	activeTile = playerDeadTiles;
}