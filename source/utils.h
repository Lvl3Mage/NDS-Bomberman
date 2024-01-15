#ifndef Utils_H
#define Utils_H
float lerp(float a, float b, float t);

int RandomRange(int min, int max);

int RandomRangeSeeded(int min, int max, int seed);

float ValueNoise(float scale, float x, float y, int repeatDistance, int min, int max);
#endif