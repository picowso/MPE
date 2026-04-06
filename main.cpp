// main.cpp: merges SDL3 with physics.cpp
// should be moved to /examples and physics.cpp being a header only lib!!!
#include "header.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const int n_pnts = 500;
SDL_FPoint points[n_pnts], velocity[n_pnts];
float mass[n_pnts];
vector<array<int, 3>> clrs;
const float epsilon = 1.f;
float alpha = 0.1f; // step

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");
    clrs.resize(n_pnts);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    for(int i = 0 ; i < n_pnts ; i++) {
    	points[i].x = SDL_randf() * ((float) WINDOW_WIDTH);
    	points[i].y = SDL_randf() * ((float) WINDOW_HEIGHT);
    	velocity[i].x = 0.f;
    	velocity[i].y = 0.f;
    	mass[i] = 1.f;
    	for(int j = 0 ; j < 3 ; j++) clrs[i][j] = rand()%256;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// float dis2(SDL_FPoint &a, SDL_FPoint &b) {
// 	float dx = a.x - b.x;
// 	float dy = a.y - b.y;
// 	return dx*dx + dy*dy;
// }

float angle(SDL_FPoint &a, SDL_FPoint &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return atan2(dy, dx);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
	// |F| = \sum 1/r^2
    for(int i = 0 ; i < n_pnts ; i++) {
    	float ddx = 0., ddy = 0.;
    	for(int j = 0 ; j < n_pnts ; j++) {
    		if(j == i) continue;
    		float dx = points[j].x - points[i].x;
			float dy = points[j].y - points[i].y;
    		float dis2 = dx*dx + dy*dy + epsilon;
    		float inv = 1./sqrtf(dis2);
    		float inv3 = mass[j] * inv * inv * inv;
    		ddx += dx * inv3;
    		ddy += dy * inv3;
    	}

    	velocity[i].x += ddx * alpha;
    	velocity[i].y += ddy * alpha;
    }

    for(int i = 0 ; i < n_pnts ; i++) {
    	points[i].x += velocity[i].x * alpha;
    	points[i].y += velocity[i].y * alpha;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    // SDL_RenderPoints(renderer, points, SDL_arraysize(points));  /* draw all the points! */

    for(int i = 0 ; i < n_pnts ; i++) {
    	if(points[i].x > WINDOW_WIDTH or points[i].x < 0) velocity[i] = {0.f, 0.f};
    	if(points[i].y > WINDOW_HEIGHT or points[i].y < 0) velocity[i] = {0.f, 0.f};

    	SDL_SetRenderDrawColor(renderer, clrs[i][0], clrs[i][1], clrs[i][2], SDL_ALPHA_OPAQUE);  /* white, full alpha */
    	SDL_RenderPoint(renderer, points[i].x, points[i].y);
    }

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { /* SDL will clean up the window/renderer for us. */ }