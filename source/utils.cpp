
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