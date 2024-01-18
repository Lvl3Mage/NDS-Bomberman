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
#include "tileconfig.h"

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
int terrainTiles1;
int terrainTiles2;
int playerTiles;
int grenadeTiles;
int mudBallTiles;
int explosionTiles;
int playerDeadTiles;
unique_ptr<Scene> scene;
uint ticks = 0;
void UpdateTime(){
	ticks += timerElapsed (0);
	float ellapsedTime = ((float)ticks) / (float)TIMER_SPEED;
	if(scene==nullptr){return;}
	scene->UpdateTime(ellapsedTime);
	// scene->terrain->MarchTerrainData(50 + round(25*sin(ellapsedTime)));
}
void ApplyPalette(vector<u8*> tiles, int start){//0 - 10 to start - start+10, 10-> - 0->
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
void Clone(vector<u8*> tiles, vector<u8*> targets){
	for(int i = 0; i < tiles.size(); i++){
		for(int j = 0; j < 64; j++){
			targets[i][j] = tiles[i][j];
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

	BG_PALETTE[4]=RGB15(17,15,12);
	BG_PALETTE[5]=RGB15(8,6,4);
	BG_PALETTE[6]=RGB15(10,9,7);


	BG_PALETTE[7]=RGB15(10,10,10);
	BG_PALETTE[8]=RGB15(15,15,15);
	BG_PALETTE[9]=RGB15(19,19,19);
	BG_PALETTE[10]=RGB15(15,10,7);
	BG_PALETTE[11]=RGB15(10,4,2);
	BG_PALETTE[12]=RGB15(0,0,0);
	// BG_PALETTE[4]=RGB15(10,31,10);
	//Grenades
	BG_PALETTE[13]=RGB15(10,10,10);
	BG_PALETTE[14]=RGB15(17,17,17);
	BG_PALETTE[15]=RGB15(7,7,7);
	//Mud ball
	BG_PALETTE[16]=RGB15(17, 15, 12);
	BG_PALETTE[17]=RGB15(12, 10, 7);
	BG_PALETTE[18]=RGB15(10, 9, 7);
	//Fire

	BG_PALETTE[19]=RGB15(22, 13, 8);
	BG_PALETTE[20]=RGB15(22, 18, 10);
	BG_PALETTE[21]=RGB15(22, 22, 15);

	//
	
	BG_PALETTE[22]=RGB15(10,10,10);
	BG_PALETTE[23]=RGB15(15,15,15);
	BG_PALETTE[24]=RGB15(19,19,19);
	BG_PALETTE[25]=RGB15(15,14,13);
	BG_PALETTE[26]=RGB15(10,8,8);


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


	static u8 heavy_w_empty[64];
	static u8 heavy_w_full[64];
	static u8 heavy_w_corner1[64];
	static u8 heavy_corner2[64];
	static u8 heavy_corner3[64];
	static u8 heavy_corner4[64];
	static u8 heavy_w_halfV[64];
	static u8 heavy_w_halfV_inv[64];
	static u8 heavy_w_halfH[64];
	static u8 heavy_w_halfH_inv[64];
	static u8 heavy_w_fullCorner1[64];
	static u8 heavy_w_fullCorner2[64];
	static u8 heavy_w_fullCorner3[64];
	static u8 heavy_w_fullCorner4[64];
	static u8 heavy_w_path[64];
	static u8 heavy_w_path_inv[64];
	Clone(
		{w_empty,w_full,w_corner1,corner2,corner3,corner4,w_halfV,w_halfV_inv,w_halfH,w_halfH_inv,w_fullCorner1,w_fullCorner2,w_fullCorner3,w_fullCorner4,w_path,w_path_inv},
		{heavy_w_empty,heavy_w_full,heavy_w_corner1,heavy_corner2,heavy_corner3,heavy_corner4,heavy_w_halfV,heavy_w_halfV_inv,heavy_w_halfH,heavy_w_halfH_inv,heavy_w_fullCorner1,heavy_w_fullCorner2,heavy_w_fullCorner3,heavy_w_fullCorner4,heavy_w_path,heavy_w_path_inv}
	);

	ApplyPalette({w_empty,w_full,w_corner1,corner2,corner3,corner4,w_halfV,w_halfV_inv,w_halfH,w_halfH_inv,w_fullCorner1,w_fullCorner2,w_fullCorner3,w_fullCorner4,w_path,w_path_inv},1);

	ApplyPalette({heavy_w_empty,heavy_w_full,heavy_w_corner1,heavy_corner2,heavy_corner3,heavy_corner4,heavy_w_halfV,heavy_w_halfV_inv,heavy_w_halfH,heavy_w_halfH_inv,heavy_w_fullCorner1,heavy_w_fullCorner2,heavy_w_fullCorner3,heavy_w_fullCorner4,heavy_w_path,heavy_w_path_inv},4);
	

	//This took forever to figure out but it seems that dmaCopy works asyncrounosly? 
	//if the variables that you pass in are not static and aren't allocated on the heap they go out of scope before it executes
	//I swear this was not like this before but like this it will stay
	terrainTiles1 = DefineTiles({
		w_empty,     corner3, corner4, w_halfH_inv,
		corner2,     w_halfV_inv, w_path, w_fullCorner1,
		w_corner1,     w_path_inv, w_halfV, w_fullCorner2, 
		w_halfH, w_fullCorner4, w_fullCorner3, w_full,
	});//15
	terrainTiles2 = DefineTiles({
		heavy_w_empty,     heavy_corner3, heavy_corner4, heavy_w_halfH_inv,
		heavy_corner2,     heavy_w_halfV_inv, heavy_w_path, heavy_w_fullCorner1,
		heavy_w_corner1,     heavy_w_path_inv, heavy_w_halfV, heavy_w_fullCorner2, 
		heavy_w_halfH, heavy_w_fullCorner4, heavy_w_fullCorner3, heavy_w_full,
	});


	ApplyPalette({dwarf},7);
	//player
	playerTiles = DefineTiles({dwarf});//16


	//Projectiles
	ApplyPalette({grenade_H, grenade_H_inv,grenade_V,grenade_V_inv},13);

	grenadeTiles = DefineTiles({grenade_H, grenade_H_inv,grenade_V,grenade_V_inv});//21


	ApplyPalette({mud_ball1},16);
	static u8 mud_ball2[64];
	MirrorX(mud_ball1,mud_ball2);
	static u8 mud_ball3[64];
	MirrorY(mud_ball2,mud_ball3);
	static u8 mud_ball4[64];
	MirrorY(mud_ball1,mud_ball4);


	mudBallTiles = DefineTiles({mud_ball1, mud_ball2, mud_ball3, mud_ball4});

	ApplyPalette({expl8, expl7, expl6, expl5, expl4, expl3, expl2, expl1},19);

	explosionTiles = DefineTiles({expl8, expl7, expl6, expl5, expl4, expl3, expl2, expl1});


	ApplyPalette({dwarf_dead},22);
	

	playerDeadTiles = DefineTiles({dwarf_dead});
}


int main()
{

	srand(static_cast<unsigned int>(time(0)));
	Init();
	iprintf("\x1b[12;2H Loading Scene...");

	scene = make_unique<Scene>(mapMemory, ticks);

	timerStart (0 , ClockDivider_1024 , 0 , NULL ) ; 
	while(1)
	{
    	consoleClear();
		scanKeys();
		UpdateTime();
		scene->Update();
		if(scene->AlivePlayerCount() <=1){
			u32 key = keysDown();
			if(key & KEY_X){
				consoleClear();
				iprintf("\x1b[12;2H Loading Scene...");
				scene = make_unique<Scene>(mapMemory, ticks);

			}
		}

		swiWaitForVBlank();
	}
}

