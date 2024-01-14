#include <nds.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "tiles.h"
#include "world.h"
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
int camX=0;
int camY=0;
const int camWidth = 32;
const int camHeight = 24;

int WALL_TILES_INDEX;

unique_ptr<World> world;
float worldTime = 0;
void UpdateTime(){
	worldTime+=0.1;
	if(world == nullptr){
		return;
	}
	world->MarchWorldData(50 + round(25*sin(worldTime)), WALL_TILES_INDEX);
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
void RenderWorld()
{
	for (int screenY = 0; screenY < camHeight; screenY++)
	{
		for (int screenX = 0; screenX < camWidth; screenX++)
		{
			int coordX = screenX+camX;
			int coordY = screenY+camY;
			int pos_mapMemory = (camHeight-1-screenY) * camWidth + screenX; //y coord inverted
			mapMemory[pos_mapMemory] = world->GetWorldAt(coordX,W_SIZE-1-coordY); // world sampling inverted
		}
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


	// BG_PALETTE[3]=RGB15(31,31,0);
	// BG_PALETTE[4]=RGB15(10,31,10);

	u8 corner2[64];
	MirrorX(w_corner1,corner2);
	u8 corner3[64];
	MirrorY(corner2,corner3);
	u8 corner4[64];
	MirrorY(w_corner1,corner4);

	u8 w_halfV_inv[64];
	MirrorX(w_halfV,w_halfV_inv);

	u8 w_halfH_inv[64];
	MirrorY(w_halfH,w_halfH_inv);

	u8 w_fullCorner2[64];
	MirrorX(w_fullCorner1,w_fullCorner2);
	u8 w_fullCorner3[64];
	MirrorY(w_fullCorner2,w_fullCorner3);
	u8 w_fullCorner4[64];
	MirrorY(w_fullCorner1,w_fullCorner4);


	u8 w_path_inv[64];
	MirrorY(w_path,w_path_inv);

	WALL_TILES_INDEX = DefineTiles({
		w_empty,     corner2, w_corner1, w_halfH,
		corner3,     w_halfV_inv, w_path_inv, w_fullCorner4,
		corner4,     w_path, w_halfV, w_fullCorner3, 
		w_halfH_inv, w_fullCorner1, w_fullCorner2, w_full,
	});
}
float ValueNoise(float scale, float x, float y, int repeatDistance, int min, int max){
	float localX = x/scale;
	float localY = y/scale;


	int squareCoordX = floor(localX);
	int squareCoordY = floor(localY);

	float xTravel = localX - squareCoordX;
	float yTravel = localY - squareCoordY;


	//Coordinates of the square vertecies
	int posIndex = squareCoordX+squareCoordY*repeatDistance;
	int posIndexT = squareCoordX+(squareCoordY+1)*repeatDistance;
	int posIndexR = squareCoordX+1+squareCoordY*repeatDistance;
	int posIndexTR = squareCoordX+1+(squareCoordY+1)*repeatDistance;


	float bottom = lerp((float)RandomRangeSeeded(min,max,posIndex),(float)RandomRangeSeeded(min,max,posIndexR), xTravel);
	float top = lerp((float)RandomRangeSeeded(min,max,posIndexT),(float)RandomRangeSeeded(min,max,posIndexTR), xTravel);
	return lerp(top,bottom,yTravel);
}
template<int WorldSize>
void GenerateWorldData(int dataTarget[WorldSize+1][WorldSize+1]){
	for(int i=0; i<WorldSize+1;i++){
		for(int j=0; j<WorldSize+1;j++){
			// float x = i/2.0;
			// float y = j/2.0;
			// dataTarget[i][j] = round(i%5)-1;
			// dataTarget[i][j] = RandomRange(-1,2);
			// dataTarget[i][j] = round(cos(2*x+sin(y))*sin(cos(x)+y/2.0));
			//circles
			// int size = 26;
			// int x = i%size - size/2;
			// int y = j%size - size/2;
			// dataTarget[i][j] = round(sqrt(x*x+y*y)-size/3 );
			dataTarget[i][j] = ValueNoise(12,i,j,W_SIZE+1,0,100);
		}
	}
}


int main()
{
	srand(static_cast<unsigned int>(time(0)));
	Init();
	iprintf("\x1b[12;2H Loading...");
	int worldData[W_SIZE+1][W_SIZE+1] = {0};
	GenerateWorldData<W_SIZE>(worldData);

	world = make_unique<World>(worldData);
	world->MarchWorldData(50,WALL_TILES_INDEX);

	iprintf("\x1b[12;2H Loaded!   ");

	while(1)
	{
		u32 key;
		scanKeys();
		key = keysDown();
		if (key & KEY_RIGHT) {
			camX ++;
		}
		if (key & KEY_LEFT){
			camX --;
		}
		if (key & KEY_UP) {
			camY ++;
		}
		if (key & KEY_DOWN){
			camY --;
		}
		RenderWorld();
		swiWaitForVBlank();
	}
}

