#pragma once

#include <linmath.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"

#include "entity.h"

typedef struct ground {
	Entity base;

	float w, h;
	bool fill;
	// rgba
	vec4 color;
} Ground;

Ground* GroundCreateGround(float x, float y, float w, float h, bool fill, vec4 color);
void GroundRenderGround(SDL_Renderer* renderer, Ground* ground);