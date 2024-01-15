#ifndef Player_H
#define Player_H

class Player;


#include "config.h"
#include "terrain.h"
using namespace std;
class Player
{
public:
	int x=0;
	int y=0;

	Player(int startX, int startY);
	// Update(unique_ptr<Scene>& terrain){

	// }
};
#endif