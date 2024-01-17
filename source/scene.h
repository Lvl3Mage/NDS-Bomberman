#ifndef Scene_H
#define Scene_H

class Scene;
struct Explosion;
#include <vector>
#include <memory>
#include "config.h"
#include "terrain.h"
#include "cameracontroller.h"
#include "projectile.h"
#include "player.h"
#include "vector2.h"
using namespace std;
struct Explosion{
	Vector2 position = Vector2(0,0);
	int radius;
	int tileOffset;
	Explosion(Vector2 pos, int rad);
};
class Scene
{
	float turnTimeLeft;
	float lastFrameTime;
	void LogSceneInfo();
public:
	Scene(u16* associatedMemory);
	vector<shared_ptr<Player>> players;
	vector<shared_ptr<Projectile>> projectiles;
	unique_ptr<CameraController> cameraController;
	unique_ptr<Terrain> terrain;
	float sceneTime;
	float deltaTime;
	int activePlayerIndex = 0;
	vector<shared_ptr<Explosion>> explosions;

	void UpdateTime(float newTime);
	void Update();

	Vector2 LoopCoord(Vector2 coord);
	bool IsPlayerCollision(Vector2 coord);
	float ClosestPlayerDistance(Vector2 coord);
	void AddProjectile(shared_ptr<Projectile> projectile);
	void RemoveProjectile(Projectile* projectile);
	void NextTurn();
	float lastExplosionUpdateTime;
	const float explosionAnimTime = 0.1;
	void UpdateExplosionData();
	void AddExplosion(Vector2 position, int radius);
};
#endif