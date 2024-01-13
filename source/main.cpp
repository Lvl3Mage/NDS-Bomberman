#include <nds.h>
#include <stdio.h>
#include <vector>
using namespace std;

u8 black_bg[64] =
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

u8 wall[64]=
{
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1
};

u8 wall_circle[64]=
{
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0
};

u8 pacman[64]=
{
    0,0,0,3,3,0,0,0,
    0,3,3,3,3,3,0,0,
    0,3,0,3,3,3,0,0,
    3,3,3,0,0,0,0,0,
    3,3,3,0,0,0,0,0,
    0,3,3,3,3,3,0,0,
    0,3,3,3,3,3,0,0,
    0,0,0,3,3,0,0,0
};

u8 ghost[64]=
{
    0,0,0,4,4,0,0,0,
    0,0,4,4,4,4,0,0,
    0,4,4,4,4,4,4,0,
    0,4,0,4,4,0,4,0,
    0,4,4,4,4,4,4,0,
    0,4,4,4,4,4,4,0,
    0,4,4,4,4,4,4,0,
    0,4,0,4,4,0,4,0
};

u8 coin[64]=
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,2,2,0,0,0,
    0,0,2,2,2,2,0,0,
    0,0,2,2,2,2,0,0,
    0,0,0,2,2,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};


// u16 mapData[768] =
// {
// 	X,X,X,X, X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X, X,X,X,X,X,
// 	X,5,5,5, X,5,5,5,X,5,5,5,5,5,5,5,5,5,5,5,5,5,X,5,5,5,5, 5,5,5,5,X,
// 	X,5,X,5, X,5,2,5,X,5,X,X,X,X,X,5,X,X,X,X,X,5,X,5,2,5,X, X,5,2,5,X,
//     X,5,X,5, X,5,5,5,5,5,5,5,X,5,5,5,5,5,X,5,5,5,5,5,5,5,X, X,5,5,5,X,

//     X,5,X,5, X,X,X,5,X,5,X,5,X,5,X,X,X,5,X,5,X,5,X,5,X,X,X, X,X,5,X,X,
//     X,5,X,5, X,5,5,5,X,5,X,5,X,5,X,0,X,5,X,5,X,5,X,5,5,5,X, 5,5,5,5,X,
//     X,5,X,5, X,5,2,5,X,5,X,5,X,5,X,X,X,5,X,5,X,5,X,5,2,5,X, 5,X,X,5,X,
//     X,5,X,5, X,5,5,5,X,5,X,5,5,5,5,5,5,5,5,5,X,5,X,5,5,5,X, 5,5,X,5,X,

//     X,5,X,5, X,X,X,X,X,5,X,5,X,X,X,X,X,X,X,5,X,5,X,X,X,X,X, 5,X,X,5,X,
//     X,5,X,5, X,5,5,5,5,X,5,5,5,5,5,5,5,5,5,5,X,5,5,5,5,5,X, 5,0,0,5,X,
//     X,5,X,5, X,5,X,X,X,X,X,5,X,X,X,0,X,X,X,5,X,X,X,X,X,5,X, 5,0,0,5,X,
//     X,5,5,5, 5,5,5,5,5,5,5,5,X,0,0,4,0,0,X,5,5,5,5,5,5,5,5, 5,5,5,5,X,

//     X,5,X,5, X,5,X,X,X,X,X,5,X,X,X,X,X,X,X,5,X,X,X,X,X,5,X, 0,0,0,0,X,
//     X,5,X,5, X,5,5,5,5,X,5,5,5,5,5,5,5,5,5,5,X,5,5,5,5,5,X, 0,0,0,0,X,
//     X,5,X,5, X,X,X,X,X,5,X,5,X,X,X,X,X,X,X,5,X,5,X,X,X,X,X, 0,0,0,0,X,
//     X,5,X,5, X,5,5,5,X,5,X,5,5,5,5,5,5,5,5,5,X,5,X,5,5,5,X, 0,0,0,0,X,

//     X,5,X,5, X,5,2,5,X,5,X,5,X,5,X,X,X,5,X,5,X,5,X,5,2,5,X, 0,0,0,0,X,
//     X,5,X,5, X,5,5,5,X,5,X,5,X,5,X,0,X,5,X,5,X,5,X,5,5,5,X, 0,0,0,0,X,
//     X,5,X,5, X,X,X,5,X,5,X,5,X,5,X,X,X,5,X,5,X,5,X,5,X,X,X, 0,0,0,0,X,
//     X,5,X,5, X,5,5,5,5,5,5,5,X,5,5,5,5,5,X,5,5,5,5,5,5,5,X, 0,0,0,0,X,

//     X,5,X,5, X,5,X,5,X,5,X,5,5,5,X,5,X,5,5,5,X,5,X,5,X,5,X, 0,0,0,0,X,
//     X,5,X,5, X,5,X,5,X,5,X,X,X,X,X,3,X,X,X,X,X,5,X,5,X,5,X, 0,0,0,0,X,
// 	X,5,5,5, X,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,X, 0,0,0,0,X,
//     X,X,X,X, X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X, X,X,X,X,X
// };
u16 mapData[768] =
{
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,2,9,9, 9,9,2,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,

    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,2,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,

    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,

    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,

    9,9,9,9, 9,9,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,2,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,

    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,3,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,
    9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9
};
int CoordToIndex(int x, int y){
	return y*32 + x;
}
int* IndexToCoord(int index){
	int* coord = new int[2]{index % 24,index / 24};
	return coord;
}
int SearchedVal = 21;
int PassableVal[2] = {5,0};
// int PassableValLen = 2;
bool FindPathToValue(int* searchMap, int x, int y, int searchVal){
	if(searchMap[CoordToIndex(x,y)] == searchVal){return true;}
	searchMap[CoordToIndex(x,y)] = SearchedVal;
	int coord[2];
	for(int i=-1; i <= 1; i++){
		coord[0] = x+i;
		if(coord[0] < 0 || coord[0] >= 24){
			continue;
		}
		for(int j = -1; j <= 1; j++){
			coord[1] = y+j;
			if(coord[1] < 0 || coord[1] >= 24 || (j == 0 && i == 0)){
				continue;
			}
			int val = searchMap[CoordToIndex(coord[0],coord[1])];
			for(int passIndex = 0; passIndex < sizeof(PassableVal) / sizeof(PassableVal[0]); passIndex++){
				if(val == PassableVal[passIndex]){
					bool pathFound = FindPathToValue(searchMap ,coord[0], coord[1], searchVal);
					if(pathFound){
						return true;
					}
					break;
				}
			}
		}
	}
	return false;
}
int* CopyMap(){
	int* mapCopy = new int[24*32];
	for(int i=0; i < 768; i++){
		mapCopy[i] = mapData[i];
	}
	return mapCopy;
}
void GenerateCell(int* generationMap, int x, int y){

	int coord[2];
	if(mapData[CoordToIndex(x,y)] == 9){
		bool pathFound = true;
		for(int i=-1; i <= 1; i++){
			coord[0] = x+i;
			if(coord[0] < 0 || coord[0] >= 24){
				continue;
			}
			for(int j = -1; j <= 1; j++){
				coord[1] = y+j;
				if(coord[1] < 0 || coord[1] >= 24 || (j == 0 && i == 0)){
					continue;
				}
				int* searchMap = CopyMap();
				if(!FindPathToValue(searchMap,coord[0], coord[1], 3)){
					pathFound = false;
					break;
				}
				delete searchMap;
			}
		}
		mapData[CoordToIndex(x,y)] = pathFound ? X : 5;
	}
	generationMap[CoordToIndex(x,y)] = 1;
	// mapData[CoordToIndex(x,y)] = 5;

	coord[0] = x;
	coord[1] = y;
	// if(coord[1] <0){
	// 	return;
	// }
	// GenerateCell(generationMap,coord[0], coord[1]);
	for(int i=-1; i <= 1; i++){
		coord[0] = x+i;
		if(coord[0] < 0 || coord[0] >= 32){
			continue;
		}
		for(int j = -1; j <= 1; j++){
			coord[1] = y+j;
			if(coord[1] < 0 || coord[1] >= 24 || (j == 0 && i == 0)){
				continue;
			}

			if(generationMap[CoordToIndex(coord[0], coord[1])] == 0){
				GenerateCell(generationMap, coord[0], coord[1]);
			}
		}
	}
}
int main( void )
{
	int* mapCopy = new int[24*32];
	GenerateCell(mapCopy, 14, 21);

	// mapData[CoordToIndex(14,21)] = 5;
	int row,col,pos_mapMemory,pos_mapData;

	REG_POWERCNT = (vu16) POWER_ALL_2D;
	REG_DISPCNT  = MODE_0_2D | DISPLAY_BG0_ACTIVE ;
	VRAM_A_CR    = VRAM_ENABLE | VRAM_A_MAIN_BG ;
	BGCTRL [0]   = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);

	static u8*  tileMemory = (u8*)  BG_TILE_RAM(1);
	static u16* mapMemory  = (u16*) BG_MAP_RAM(0);

	BG_PALETTE[0]=RGB15(5,5,5);
	BG_PALETTE[1]=RGB15(10,10,31);
	BG_PALETTE[2]=RGB15(31,15,10);
	BG_PALETTE[3]=RGB15(31,31,0);
	BG_PALETTE[4]=RGB15(10,31,10);

	dmaCopy(black_bg,    tileMemory     ,  sizeof(black_bg));     // 0
	dmaCopy(wall,        tileMemory + 64,  sizeof(wall));         // 1
	dmaCopy(wall_circle, tileMemory + 128, sizeof(wall_circle));  // 2
	dmaCopy(pacman,      tileMemory + 192, sizeof(pacman));       // 3
	dmaCopy(ghost,       tileMemory + 256, sizeof(ghost));        // 4
	dmaCopy(coin,        tileMemory + 320, sizeof(coin));         // 5

	pos_mapData = 0;
	for(row=0;row<24;row++)
		for(col=0;col<32;col++)
		{
			pos_mapMemory            = row*32+col;
			mapMemory[pos_mapMemory] = mapData[pos_mapData];
			pos_mapData ++;
	    }

	while(1)
	{
		swiWaitForVBlank();
	}
}