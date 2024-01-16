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
void Player::UpdateMovement(Scene* scene){

	u32 key;
	scanKeys();
	key = keysCurrent();
	if (key & KEY_RIGHT) {
		moveDelta.x = 1;
	}
	if (key & KEY_LEFT){
		moveDelta.x = -1;
	}
	
	if (key & KEY_UP) {
		moveDelta.y = 1;
	}
	if (key & KEY_DOWN){
		moveDelta.y = -1;
	}
	



	if(scene->sceneTime < lastMoveTime + moveCooldown){
		return;
	}
	lastMoveTime = scene->sceneTime;

	Vector2 nextPos = Vector2(moveDelta.x + position.x, moveDelta.y + position.y);
	nextPos = scene->LoopCoord(nextPos);

	if(!scene->terrain->IsTerrainAt(nextPos.x, position.y)){
		position.x = nextPos.x;
	}
	moveDelta.x = 0;

	if(!scene->terrain->IsTerrainAt(position.x, nextPos.y)){
		position.y = nextPos.y;
	}
	moveDelta.y = 0;
	
}
void Player::Update(Scene* scene){

}