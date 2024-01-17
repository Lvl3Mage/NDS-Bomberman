#include "projectile.h"
#include "terrain.h"
#include "config.h"
#include "utils.h"
#include "scene.h"
#include <memory>
#include <cmath>
#include <algorithm>


Projectile::Projectile(Vector2 pos, Vector2 dir, float vel){
	position = pos;
	direction = dir;
	velocity = vel;
}
void Projectile::Update(Scene* scene){
	accumulatedDistance += scene->deltaTime*velocity;
	activeTile = 17+GetTileOffset();
	
	if(accumulatedDistance > 1){
		accumulatedDistance-=1;
		position.x += direction.x;
		position.y += direction.y;
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
	scene->terrain->TerraformCircle(position, 7, -15);
	scene->RemoveProjectile(this);
}