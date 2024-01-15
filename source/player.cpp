#include "player.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include <memory>
#include <cmath>
#include <algorithm>
using namespace std;

Player::Player(int startX, int startY){
	x = startX;
	y = startY;
}