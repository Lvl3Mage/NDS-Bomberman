#ifndef Projectile_H
#define Projectile_H

class Projectile;

#include "scene.h"
#include "config.h"
#include "terrain.h"
#include "vector2.h"

class Projectile{
	float accumulatedDistance = 0;
	float accumulatedTangentDistance = 0;
	int explosionRadius;
	float terrainDamage;
	int playerDamage;
	int startTile;
	bool causeExplosion;
	int GetTileOffset();
public:
	Projectile(Vector2 pos, Vector2 dir, float vel, int _startTile, float spread, int _explosionRadius, float _terrainDamage, int _playerDamage, bool _causeExplosion);
	Vector2 position = Vector2(0,0);
	Vector2 direction = Vector2(0,0);
	float trajectoryTangentX;
	float trajectoryTangentY;
	float tangentVelocity;
	float velocity;
	void Update(Scene* scene);
	void OnCollision(Scene* scene);
	u8 activeTile;
};
#endif