
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;


float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}
int RandomRange(int min, int max){
	return min + rand() % (max + 1);
}
int RandomRangeSeeded(int min, int max, int seed){
	srand(static_cast<unsigned int>(seed));
	return min + rand() % (max + 1);
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