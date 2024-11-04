#include <math.h>
#include <stdlib.h>
#include "player.h"
#include "physics.h"

const float FLT_MAX = 340282346638528859811704183484516925440.0000000000000000f;

void PlayerCapSpeed(Player* p) {
	//float speed = sqrtf(p->velocity[0] * p->velocity[0] + p->velocity[1] * p->velocity[1]);
	float speed = fabsf(p->velocity[0]);
	if (speed > p->maxspd) {

		float scale = p->maxspd / speed;
		p->velocity[0] *= scale;
	}
}

Player* PlayerInitPlayer(float x, float y, float w, float h, float accel, float decel, float maxspd, float gravity, SDL_Rect hitbox, float jumpheight) {
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	if (!newPlayer)
		ERROR_RETURN(NULL, "Couldn't allocate memory for new player.\n");

	newPlayer->base.x = x;
	newPlayer->base.y = y;
	newPlayer->w = w;
	newPlayer->h = h;
	newPlayer->velocity[0] = 0;
	newPlayer->velocity[1] = 0;
	newPlayer->accel = accel;
	newPlayer->decel = decel;
	newPlayer->maxspd = maxspd;
	newPlayer->gravity = gravity;

	newPlayer->base.hitbox = hitbox;

	newPlayer->left = false;
	newPlayer->grounded = false;
	newPlayer->jumping = false;

	newPlayer->jumpheight = jumpheight;
	newPlayer->currentjump = 0.0f;

	return newPlayer;
}

void PlayerCollide(Player* p, EntityList* elist, int passes) {
	SDL_Rect pHitbox = {
		(int)p->base.hitbox.x,
		(int)p->base.hitbox.y,
		(int)p->base.hitbox.w,
		(int)p->base.hitbox.h
	};
	for (size_t i = 0; i < elist->size; i++) {

		Entity* e = elist->entities[i];

		if (e->type != ENTITY_GROUND) continue;

		SDL_Rect eHitbox = {
			(int)e->hitbox.x,
			(int)e->hitbox.y,
			(int)e->hitbox.w,
			(int)e->hitbox.h,
		};

		SDL_Rect intersect = { 0,0,0,0 };

		if (SDL_IntersectRect(&pHitbox, &eHitbox, &intersect)) {

			if (intersect.h < intersect.w) {
				if (p->velocity[1] > 0) {
					p->base.hitbox.y -= intersect.h;
					p->base.y = p->base.hitbox.y;
					p->grounded = true;
					p->jumping = false;
				}
				else if (p->velocity[1] < 0) {
					p->base.hitbox.y += intersect.h;
					p->base.y = p->base.hitbox.y;
				}
				p->velocity[1] = 0;
			}
			else {
				if (p->velocity[0] > 0) {
					p->base.hitbox.x -= intersect.w;
					p->base.x = p->base.hitbox.x;
				}
				else if (p->velocity[0] < 0) {
					p->base.hitbox.x += intersect.w;
					p->base.x = p->base.hitbox.x;
				}
				p->velocity[0] = 0;
			}
		}

	}
}

void PlayerCheckGrounded(Player* p, EntityList* elist) {
	SDL_Rect lower = p->base.hitbox;
	lower.y += 1;
	lower.w -= 1;
	lower.x += 1;

	p->grounded = false;

	for (size_t i = 0; i < elist->size; i++) {
		Entity* e = elist->entities[i];

		if (e->type != ENTITY_GROUND) continue;

		SDL_Rect eHitbox = {
			(int)e->hitbox.x,
			(int)e->hitbox.y,
			(int)e->hitbox.w,
			(int)e->hitbox.h,
		};

		if (SDL_HasIntersection(&lower, &eHitbox)) {
			p->grounded = true;
			break;
		}
	}
}

void PlayerApplyPhysics(Player* p, EntityList* elist, float dt) {
	PlayerCheckGrounded(p, elist);
	PhysicsApplyGravityPlayer(p, dt);
	PlayerCollide(p, elist, 2);
}

void PlayerJump(Player* p) {
	if (!p->grounded || p->jumping) return;
	
	p->velocity[1] = -sqrtf(p->gravity * p->jumpheight);
	p->jumping = true;
	p->grounded = false;
}

void PlayerInput(Player* p) {

	const Uint8* _currentKeys = SDL_GetKeyboardState(NULL);

	float damping = 0.9f;

	if (_currentKeys[SDL_SCANCODE_LEFT]) {
		p->velocity[0] -= p->accel;
		p->left = true;
		//DEBUG_PRINT("left\n");
	}
	if (_currentKeys[SDL_SCANCODE_RIGHT]) {
		p->velocity[0] += p->accel;
		p->left = false;
		//DEBUG_PRINT("right\n");
	}
	else {
		if (p->velocity[0] > 0) {
			p->velocity[0] -= p->decel;
			if (p->velocity[0] < 0) p->velocity[0] = 0;
		}
		if (p->velocity[0] < 0) {
			p->velocity[0] += p->decel;
			if (p->velocity[0] > 0) p->velocity[0] = 0;
		}
	}

	if (_currentKeys[SDL_SCANCODE_UP]) {
		//pjump = true;
		PlayerJump(p);
	}

	PlayerCapSpeed(p);
}

void PlayerUpdate(Player* p, float dt, EntityList* elist) {
	p->base.x += (p->velocity[0] * dt);
	p->base.y += (p->velocity[1] * dt);

	p->base.hitbox.x = p->base.x;
	p->base.hitbox.y = p->base.y;

	DEBUG_PRINT("vel: %f, %f\tpos: %f, %f\n", p->velocity[0], p->velocity[1], p->base.x, p->base.y);

	PlayerApplyPhysics(p, elist, dt);
}
