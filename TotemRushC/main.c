#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>

// my includes :)
#include "util.h"
#include "defs.h"
#include "player.h"
#include "ground.h"
#include "wall.h"
#include "entitylist.h"

// do we have liftoff?
bool _gameRunning = false;

// window scaling
// window width, window height <-- computed at runtime!
int SCALE_FACTOR = 2;
int WW, WH;

// global window ptr
SDL_Window* _window = NULL;

// global renderer ptr
SDL_Renderer* _renderer = NULL;

// deltatime and time stuff
float _lastFrameTime = 0.0;

// temp stuff
Player* a = NULL;
Ground* g1 = NULL;
Ground* g2 = NULL;

Wall* w1 = NULL;
Wall* w2 = NULL;

const Uint8* _currentKeys;
EntityList* elist = NULL;

bool reset = false;
bool pjump = false;

void _initializeDimensions(void) {
	WW = _SCREEN_WIDTH * SCALE_FACTOR;
	WH = _SCREEN_HEIGHT * SCALE_FACTOR;
}

bool _initializeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ERROR_RETURN(false, "ERROR: Failed to initalize SDL. %s\n", SDL_GetError());
	}
	else {
		DEBUG_PRINT("Initialized SDL2.\n");
	}

	_window = SDL_CreateWindow(_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WW, WH, SDL_WINDOW_SHOWN);
	if (!_window) {
		ERROR_RETURN(false, "ERROR: Failed to create window. %s\n", SDL_GetError());
	}
	else {
		DEBUG_PRINT("Created SDL2 window.\n");
	}

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (!_renderer) {
		ERROR_RETURN(false, "ERROR: Failed to create renderer. %s\n", SDL_GetError());
	}
	else {
		DEBUG_PRINT("Created SDL2 renderer.\n");
	}

	return true;
}

void _clear(void) {
	SDL_SetRenderDrawColor(_renderer, _WINDOW_DEFAULT_COLOR);

	SDL_RenderClear(_renderer);
}

void _close(void) {
	// destroy game objects
	EntityListFree(elist);

	SDL_DestroyRenderer(_renderer);
	DEBUG_PRINT("Destroyed SDL2 renderer.\n");
	SDL_DestroyWindow(_window);
	DEBUG_PRINT("Destroyed SDL2 window.\n");
	IMG_Quit();
	DEBUG_PRINT("Quit IMG.\n");
	SDL_Quit();
	DEBUG_PRINT("Quit SDL2.\n\nBye bye!\n");
}

// for now just sets default window color
void _setup() {
	_clear();

	SDL_RenderPresent(_renderer);

	
	a = PlayerInitPlayer(WW * 0.5, WH * 0.3, 20 * SCALE_FACTOR, 20 * SCALE_FACTOR, 0.75f, 0.35f, 220.0f, 500.0f, (SDL_Rect) { WW * 0.5, WH * 0.5, 20 * SCALE_FACTOR, 20 * SCALE_FACTOR }, 100.0f);
	g1 = GroundCreateGround(20 * SCALE_FACTOR, WH * 0.8, WW - 40 * SCALE_FACTOR, 20 * SCALE_FACTOR, true, (vec4) { 0, 100, 100, 255 });
	g2 = GroundCreateGround(20 * SCALE_FACTOR, WH * 0.4, WW - 40 * SCALE_FACTOR, 20 * SCALE_FACTOR, true, (vec4) { 0, 100, 100, 255 });
	w1 = WallCreateWall(20 * SCALE_FACTOR, 20 * SCALE_FACTOR, 20 * SCALE_FACTOR, WH * 0.6, true, (vec4) { 0, 100, 100, 255 });
	w2 = WallCreateWall(WW - 40 * SCALE_FACTOR, 20 * SCALE_FACTOR, 20 * SCALE_FACTOR, WH * 0.6, true, (vec4) { 0, 100, 100, 255 });
	elist = EntityListInitialize(20);

	EntityListAddEntity(elist, (Entity*)g1);
	EntityListAddEntity(elist, (Entity*)g2);
	EntityListAddEntity(elist, (Entity*)w1);
	EntityListAddEntity(elist, (Entity*)w2);


	//EntityListAddEntity(elist, (Entity*)a);
}

// game loop funcs
void _processInput() {
	// process sdl_events
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type) {
		// pressed exit button?
	case SDL_QUIT:
		_gameRunning = false;
		break;
		// any keypress
	case SDL_KEYDOWN:
		// keysym.sym is the symbol emitted by the keypress, depends on keyboard layout. i.e. pressing A on AZERTY keyb gives 'a'.
		// keysym.scancode is the symbol emitted by the keypress based on the location of the key. i.e. pressing A on AZERTY keyb gives 'Q'.
		// stay in school kids!
		switch (e.key.keysym.sym) {
			// specifically the esc key
		case SDLK_ESCAPE:
			_gameRunning = false;
			break;
			break;
		default:
			break;
		}
	case SDL_KEYUP:

		break;
	default:
		break;
	}

	// todo: move to PlayerInput()
	PlayerInput(a);
}
void _update() {
	float dt = (SDL_GetTicks64() - _lastFrameTime) / 1000.0f;

	_lastFrameTime = SDL_GetTicks64();

	// todo: move to PlayerUpdate()
	PlayerUpdate(a, dt, elist);

}
void _render() {
	_clear();

	GroundRenderGround(_renderer, g1);
	GroundRenderGround(_renderer, g2);
	GroundRenderGround(_renderer, w1);
	GroundRenderGround(_renderer, w2);

	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

	// todo: move to PlayerRender()

	PlayerRender(a, _renderer);

	

	SDL_RenderPresent(_renderer);
}

int main(int argc, char* argv[]) {
	puts("-/-/-/-/-/-/-/-/-				Totem Rush (C Rewrite)				-\\-\\-\\-\\-\\-\\-\\-\\-\n\n");

	_initializeDimensions();
	_gameRunning = _initializeWindow();

	_setup();

	while (_gameRunning) {
		_processInput();
		_update();
		_render();
	}

	_close();

	return 0;
}