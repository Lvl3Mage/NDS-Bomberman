#include "projectile.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include <memory>
#include <cmath>
#include <algorithm>



Projectile::Projectile(Vector2 pos, Vector2 dir, float vel, int _startTile, float spread, int _explosionRadius, float _terrainDamage, int _playerDamage, bool _causeExplosion){
	position = pos;
	direction = dir;
	velocity = vel;
	explosionRadius = _explosionRadius;
	terrainDamage = _terrainDamage;
	playerDamage = _playerDamage;
	startTile = _startTile;
	spread  = 1 / spread;
	causeExplosion = _causeExplosion;
	if(direction.y == 0){
		trajectoryTangentY = RandomRange(-vel/spread, vel/spread);
	}
	if(direction.x == 0){
		trajectoryTangentX = RandomRange(-vel/spread, vel/spread);
	}
}
void Projectile::Update(Scene* scene){
	accumulatedDistance += scene->deltaTime*velocity;
	accumulatedTangentDistance += scene->deltaTime*(abs(trajectoryTangentX)+abs(trajectoryTangentY));

	activeTile = startTile+GetTileOffset();
	
	if(accumulatedDistance > 1){
		accumulatedDistance-=1;
		position.x += direction.x;
		position.y += direction.y;

		position = scene->LoopCoord(position);
	}

	if(accumulatedTangentDistance > 1){
		accumulatedTangentDistance-=1;
		position.x += sign(trajectoryTangentX);
		position.y += sign(trajectoryTangentY);

		position = scene->LoopCoord(position);
	}

	if(scene->IsPlayerCollision(position) || scene->terrain->IsTerrainAt(position)){
		OnCollision(scene);
	}
}
int Projectile::GetTileOffset(){
	if(direction.x > 0) return 0;
	if(direction.x < 0) return 1;
	if(direction.y > 0) return 2;
	if(direction.y < 0) return 3;
	return 0;
}
void Projectile::OnCollision(Scene* scene){
	vector<shared_ptr<Player>> players = scene->players;
	for(int i = 0; i < players.size(); i++){
		Vector2 delta = Vector2(players[i]->position.x - position.x, players[i]->position.y - position.y);
		if(sqrt(delta.x*delta.x + delta.y*delta.y)){
			players[i]->Damage(playerDamage);
		}
	}
	if(causeExplosion){
		scene->AddExplosion(position, explosionRadius-1);
	}
	scene->terrain->TerraformCircle(scene,position, explosionRadius, terrainDamage);
	scene->RemoveProjectile(this);
}