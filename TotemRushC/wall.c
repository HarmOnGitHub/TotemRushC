#include "wall.h"

Wall* WallCreateWall(float x, float y, float w, float h, bool fill, vec4 color) {
	Wall* newGround = (Wall*)malloc(sizeof(Wall));

	if (!newGround)
		ERROR_RETURN(NULL, "Could not allocate memory for object of type Wall.\n");

	newGround->base.type = ENTITY_GROUND;

	newGround->base.x = x;
	newGround->base.y = y;
	newGround->w = w;
	newGround->h = h;
	newGround->fill = fill;

	newGround->base.hitbox.x = x;
	newGround->base.hitbox.y = y;
	newGround->base.hitbox.w = w;
	newGround->base.hitbox.h = h;

	newGround->color[0] = color[0];
	newGround->color[1] = color[1];
	newGround->color[2] = color[2];
	newGround->color[3] = color[3];

	return newGround;
}

void WallRenderWall(SDL_Renderer* renderer, Wall* ground) {
	SDL_SetRenderDrawColor(renderer, ground->color[0], ground->color[1], ground->color[2], ground->color[3]);

	if (ground->fill) {
		//fill rect
		SDL_RenderFillRect(renderer, &ground->base.hitbox);
	}
	else {
		//outline rect
		SDL_RenderDrawRect(renderer, &ground->base.hitbox);
	}
}