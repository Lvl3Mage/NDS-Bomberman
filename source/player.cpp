#include "player.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;

void Player::ClampPosition(){
	x = clamp(x, 0, W_SIZE);
	y = clamp(y, 0, W_SIZE);
}
Player::Player(int startX, int startY){
	x = startX;
	y = startY;
}
void Player::Update(Scene* scene){
	u32 key;
	scanKeys();
	key = keysCurrent();
	int newPosX = x;
	int newPosY = y;
	if (key & KEY_RIGHT) {
		newPosX ++;
	}
	if (key & KEY_LEFT){
		newPosX --;
	}
	if (key & KEY_UP) {
		newPosY ++;
	}
	if (key & KEY_DOWN){
		newPosY --;
	}
	if(!scene->terrain->IsTerrainAt(newPosX, newPosY)){
		x = newPosX;
		y = newPosY;
	}
}