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

	if(accumulatedDistance > 1){
		accumulatedDistance-=1;
		position.x += direction.x;
		position.y += direction.y;
	}
	if(scene->IsPlayerCollision(position) || scene->terrain->IsTerrainAt(position)){
		OnCollision(scene);
	}
}
void Projectile::OnCollision(Scene* scene){}