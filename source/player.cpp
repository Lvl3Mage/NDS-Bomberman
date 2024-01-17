#include "player.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;


Player::Player(int startX, int startY){
	position = Vector2(startX, startY);
}
void Player::ActiveUpdate(Scene* scene){

	Vector2 actionDir = Vector2(0,0);
	u32 key;
	scanKeys();
	key = keysCurrent();
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
	
	if(key & KEY_A){//movement
		selectedActionType = 0;
	}
	if(key & KEY_L){//grenades
		selectedActionType = 1;
	}


	switch(selectedActionType){
		case 0:
			ProcessMovement(scene, actionDir);
			break;
		case 1:
			ProcessAttack(scene,actionDir);
			break;
	}
}
void Player::ProcessAttack(Scene* scene, Vector2 actionDir){

}
void Player::ProcessMovement(Scene* scene, Vector2 actionDir){
	moveDelta = actionDir;


	if(scene->sceneTime < lastMoveTime + moveCooldown){
		return;
	}
	if(remainingMovement <= 0){
		return;
	}
	if(actionDir.x == 0 && actionDir.y == 0){
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
	remainingActions = 1;
	remainingMovement = 30;
}
char* Player::GetSelectedActionName(){
	switch(selectedActionType){
		case 0:
			return "Move";
		case 1:
			return "Grenades";
	}
	return "Other";
}