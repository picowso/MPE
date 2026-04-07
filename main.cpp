// main.cpp: merges SDL3 with physics.cpp
// should be moved to /examples and physics.cpp being a header only lib!!!
#include "header.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_FPoint points[n_pnts], velocity[n_pnts];
int frame_n = 0;
float fps = 0.0f;
// vector<array<int, 3>> clrs;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("MPE", "1.0", "com.mpe.engine");
    // clrs.resize(n_pnts);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    for(int i = 0 ; i < n_pnts ; i++) {
    	points[i].x = SDL_randf() * ((float) WINDOW_WIDTH);
    	points[i].y = SDL_randf() * ((float) WINDOW_HEIGHT);
    	velocity[i].x = 0.f;
    	velocity[i].y = 0.f;
    	// for(int j = 0 ; j < 3 ; j++) clrs[i][j] = rand()%256;
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

long long last_counter = SDL_GetPerformanceCounter();
/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
	// |F| = \sum 1/r^2
    // for(int i = 0 ; i < n_pnts ; i++) quad.insert(points[i]);

    Qdtree quad;
    quad.build();
    for(int i = 0 ; i < n_pnts ; i++) {
        SDL_FPoint w = quad.vel_calc(quad.root, points[i]);
        velocity[i].x += alpha * w.x;
        velocity[i].y += alpha * w.y;
    }

    for(int i = 0 ; i < n_pnts ; i++) {
    	points[i].x += velocity[i].x * alpha;
    	points[i].y += velocity[i].y * alpha;
    }

    quad.destroy(quad.root);
    quad.root = new Node;
    quad.root->rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_RenderPoints(renderer, points, n_pnts);  /* draw all the points! */

    // for(int i = 0 ; i < n_pnts ; i++) {
    // 	if(points[i].x > WINDOW_WIDTH or points[i].x < 0) velocity[i] = {0.f, 0.f};
    // 	if(points[i].y > WINDOW_HEIGHT or points[i].y < 0) velocity[i] = {0.f, 0.f};

    // 	SDL_RenderPoint(renderer, points[i].x, points[i].y);
    // }

    // outputting FPS:
    // cout << SDL_GetPerformanceCounter() << endl;
    frame_n++;
    long long now = SDL_GetPerformanceCounter();
    double elapsed = (double)(now - last_counter) / (double)SDL_GetPerformanceFrequency();

    if (elapsed >= 0.5) { // update twice a second
        fps = (float)(frame_n / elapsed);
        frame_n = 0;
        last_counter = now;
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugTextFormat(renderer, WINDOW_WIDTH - 120, 10, "FPS: %.1f", fps);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { /* SDL will clean up the window/renderer for us. */ }