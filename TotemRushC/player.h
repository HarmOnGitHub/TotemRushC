#pragma once

#include <stdbool.h>
#include <linmath.h>
#include <SDL2/SDL.h>
#include "util.h"

#include "entity.h"
#include "entitylist.h"

typedef struct player {
	Entity base;
	float w, h;
	vec2 velocity;
	float accel, decel, maxspd;
	float gravity;
	float jumpheight;
	float currentjump;

	float jumpinc;

	bool grounded;
	bool jumping;
	bool left;
} Player;

Player* PlayerInitPlayer(float x, float y, float w, float h, float accel, float decel, float maxspd, float gravity, SDL_Rect hitbox, float jumpheight);

void PlayerCapSpeed(Player* p);

void PlayerApplyPhysics(Player* p, EntityList* elist, float dt);

void PlayerJump(Player* p);

void PlayerInput(Player* p);

void PlayerUpdate(Player* p, float dt, EntityList* elist);