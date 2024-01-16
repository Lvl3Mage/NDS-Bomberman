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
#include <time.h>
#include <cmath>
#include <vector>

#include <memory>

using namespace std;
static u8*  tileMemory;
static u16* mapMemory;

# define TIMER_SPEED ( BUS_CLOCK /1024)

int WALL_TILES_INDEX;

unique_ptr<Scene> scene;
uint ticks = 0;
void UpdateTime(){
	ticks += timerElapsed (0);
	float ellapsedTime = ((float)ticks) / (float)TIMER_SPEED;
	if(scene==nullptr){return;}
	scene->UpdateTime(ellapsedTime);
	// scene->terrain->MarchTerrainData(50 + round(25*sin(ellapsedTime)));
}
void ApplyPalette(vector<u8*> tiles, int start){//0 - 10 to start - start+10, 10> - 0>

	for(int i = 0; i < tiles.size(); i++){
		u8* tile = tiles[i];
		for(int j = 0; j < 64; j++){
			if(tile[j] < 10){
				tile[j] = start + tile[j];
			}
			else{
				tile[j] = tile[j] - 10;
			}
		}
	}
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

	// irqEnable(IRQ_TIMER0);
	// irqSet(IRQ_TIMER0, UpdateTime);
	// TIMER_DATA(0) = 62259;
	// TIMER_CR (0) = TIMER_DIV_1024 | TIMER_ENABLE | TIMER_IRQ_REQ;

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


	BG_PALETTE[4]=RGB15(10,10,10);
	BG_PALETTE[5]=RGB15(15,15,15);
	BG_PALETTE[6]=RGB15(19,19,19);
	BG_PALETTE[7]=RGB15(15,10,7);
	BG_PALETTE[8]=RGB15(10,4,2);
	BG_PALETTE[9]=RGB15(0,0,0);
	// BG_PALETTE[4]=RGB15(10,31,10);

	ApplyPalette({w_empty, w_full, w_corner1, w_halfH, w_halfV, w_fullCorner1,w_path},0);


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

	ApplyPalette({dwarf},4);

	//This took forever to figure out but it seems that dmaCopy works asyncrounosly? 
	//if the variables that you pass in are not static and aren't allocated on the heap they go out of scope before it executes
	//I swear this was not like this before but like this it will stay
	DefineTiles({
		w_empty,     corner3, corner4, w_halfH_inv,
		corner2,     w_halfV_inv, w_path, w_fullCorner1,
		w_corner1,     w_path_inv, w_halfV, w_fullCorner2, 
		w_halfH, w_fullCorner4, w_fullCorner3, w_full,
	});
	//15
	DefineTiles({dwarf});
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
	iprintf("\x1b[12;2H Loading...");

	
	
	scene = make_unique<Scene>(mapMemory);

	iprintf("\x1b[12;2H Loaded!    ");

	timerStart (0 , ClockDivider_1024 , 0 , NULL ) ; 
	while(1)
	{
    	consoleClear();
		UpdateTime();
		// scene->cameraController->Render(scene.get());
		scene->Update();
		swiWaitForVBlank();
	}
}

