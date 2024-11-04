#include "physics.h"

void PhysicsApplyGravityPlayer(Player* p, float dt) {
	if (!p->grounded)
		p->velocity[1] += p->gravity * dt;

	p->base.y += p->velocity[1] * dt;

	p->base.hitbox.y = p->base.y;
}
