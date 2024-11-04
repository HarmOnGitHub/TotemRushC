#pragma once

#include <SDL2/SDL.h>

typedef enum {
	ENTITY_PLAYER,
	ENTITY_GROUND,
	ENTITY_WALL
} EntityType;

typedef struct entity {
	EntityType type;
	float x, y;
	SDL_Rect hitbox;
} Entity;

