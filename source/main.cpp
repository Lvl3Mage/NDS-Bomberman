#include <nds.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "tiles.h"

#include "scene.h"

#include "terrain.h"
#include "player.h"
#include "cameracontroller.h"

#include "utils.h"
#include "config.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

#include <memory>

using namespace std;
static u8*  tileMemory;
static u16* mapMemory;

//Camera
int targetX = 150;
int targetY = 150;

int WALL_TILES_INDEX;

unique_ptr<Scene> scene;
float ellapsedTime = 0;
void UpdateTime(){
	ellapsedTime+=0.1;
	if(scene==nullptr){return;}
	scene->UpdateTime(ellapsedTime);
	// if(terrain == nullptr){
	// 	return;
	// }
	// terrain->MarchTerrainData(50 + round(25*sin(worldTime)), WALL_TILES_INDEX);
}
void MirrorX(u8* tile, u8* target){	
	for(int i = 0; i < 64; i++){
		int x = i % 8;
		int y = i / 8;
		int invertedIndex = y*8 + (7-x);
		target[invertedIndex] = tile[i];
	}
}
void MirrorY(u8* tile, u8* target){	
	for(int i = 0; i < 64; i++){
		int x = i % 8;
		int y = i / 8;
		int invertedIndex = (7-y)*8 + x;
		target[invertedIndex] = tile[i];
	}
}

int definedTiles = 0;
int DefineTiles(vector<u8*> tiles){
	for(int i = 0; i < tiles.size(); i++){
		u8* tile = tiles[i]; 
		dmaCopy(tile, tileMemory + definedTiles*64,  64);
		definedTiles++;
	}
	return definedTiles - tiles.size();//returns first tile index
}
void Init(){
	consoleDemoInit();

	irqEnable(IRQ_TIMER0);
	irqSet(IRQ_TIMER0, UpdateTime);
	TIMER_DATA(0) = 62259;
	TIMER_CR (0) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;

	REG_POWERCNT = (vu16) POWER_ALL_2D;
	REG_DISPCNT  = MODE_0_2D | DISPLAY_BG0_ACTIVE ;
	VRAM_A_CR    = VRAM_ENABLE | VRAM_A_MAIN_BG ;
	BGCTRL [0]   = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	tileMemory = (u8*)  BG_TILE_RAM(1);
	mapMemory  = (u16*) BG_MAP_RAM(0);


	//Main BG
	BG_PALETTE[0]=RGB15(5,5,5);
	//Wall colors
	BG_PALETTE[1]=RGB15(17,15,12);
	BG_PALETTE[2]=RGB15(12,10,7);
	BG_PALETTE[3]=RGB15(10,9,7);


	BG_PALETTE[4]=RGB15(31,31,0);
	// BG_PALETTE[4]=RGB15(10,31,10);

	static u8 corner2[64];
	MirrorX(w_corner1,corner2);
	static u8 corner3[64];
	MirrorY(corner2,corner3);
	static u8 corner4[64];
	MirrorY(w_corner1,corner4);

	static u8 w_halfV_inv[64];
	MirrorX(w_halfV,w_halfV_inv);

	static u8 w_halfH_inv[64];
	MirrorY(w_halfH,w_halfH_inv);

	static u8 w_fullCorner2[64];
	MirrorX(w_fullCorner1,w_fullCorner2);
	static u8 w_fullCorner3[64];
	MirrorY(w_fullCorner2,w_fullCorner3);
	static u8 w_fullCorner4[64];
	MirrorY(w_fullCorner1,w_fullCorner4);


	static u8 w_path_inv[64];
	MirrorY(w_path,w_path_inv);

	//This took forever to figure out but it seems that dmaCopy works asyncrounosly? 
	//if the variables that you pass in are not static and aren't allocated on the heap they go out of scope before it executes
	//I swear this was not like this before but like this it will stay
	WALL_TILES_INDEX = DefineTiles({
		w_empty,     corner2, w_corner1, w_halfH,
		corner3,     w_halfV_inv, w_path_inv, w_fullCorner4,
		corner4,     w_path, w_halfV, w_fullCorner3, 
		w_halfH_inv, w_fullCorner1, w_fullCorner2, w_full,
	});
}
template<int TerrainSize>
void GenerateTerrainData(int dataTarget[TerrainSize+1][TerrainSize+1]){
	for(int i=0; i<TerrainSize+1;i++){
		for(int j=0; j<TerrainSize+1;j++){
			dataTarget[i][j] = ValueNoise(12,i,j,W_SIZE+1,0,100);
		}
	}
}


int main()
{
	srand(static_cast<unsigned int>(time(0)));
	Init();
	iprintf("\x1b[12;2H Loading... %d", WALL_TILES_INDEX);

	
	
	scene = make_unique<Scene>(mapMemory);



	iprintf("\x1b[12;2H Loaded!   ");
	while(1)
	{
		u32 key;
		scanKeys();
		key = keysCurrent();
		if (key & KEY_RIGHT) {
			targetX ++;
		}
		if (key & KEY_LEFT){
			targetX --;
		}
		if (key & KEY_UP) {
			targetY ++;
		}
		if (key & KEY_DOWN){
			targetY --;
		}
		scene->Update();
		scene->players[0]->x = targetX;
		// cameraController->Update(scene);
		// cameraController->Render(terrain);
		swiWaitForVBlank();
	}
}

