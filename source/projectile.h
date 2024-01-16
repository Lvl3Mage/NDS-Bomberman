#ifndef Projectile_H
#define Projectile_H

class Projectile;

#include "scene.h"
#include "config.h"
#include "terrain.h"
#include "vector2.h"

class Projectile{
	float accumulatedDistance = 0;
public:
	Projectile(Vector2 coord, Vector2 dir, float vel);
	Vector2 position = Vector2(0,0);
	Vector2 direction = Vector2(0,0);
	float velocity;
	void Update(Scene* scene);
	void OnCollision(Scene* scene);
};
#endif