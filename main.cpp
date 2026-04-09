// main.cpp: merges SDL3 with physics.cpp
// should be moved to /examples and physics.cpp being a header only lib!!!
#include "header.hpp"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

// which (x,y) has point i
int where[WINDOW_WIDTH+1][WINDOW_HEIGHT+1];

SDL_FPoint points[n_pnts], velocity[n_pnts];
SDL_FPoint evil; // evil point :3
SDL_FPoint evil_o;
bool evil_a = 0;
int frame_n = 0;
float fps = 0.0f;

vector<array<int, 3>> clrs;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // memset(where, 0, sizeof where);
    clrs.resize(n_pnts);
    evil = {0,0};
    evil_o = {0,0};
    for(int i = 0 ; i < WINDOW_WIDTH ; i++) {
        for(int j = 0 ; j < WINDOW_HEIGHT ; j++) {
            where[i][j] = -1;
        }
    }

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

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    for(int i = 0 ; i < n_pnts ; i++) {
    	points[i].x = SDL_randf() * ((float) WINDOW_WIDTH);
    	points[i].y = SDL_randf() * ((float) WINDOW_HEIGHT);
        
        where[(int)points[i].x][(int)points[i].y] = i;

    	velocity[i].x = 0.f;
    	velocity[i].y = 0.f;
    	for(int j = 0 ; j < 3 ; j++) clrs[i][j] = rand()%256;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
int force = 10;
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if(event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if(event->type == SDL_EVENT_MOUSE_MOTION) {
        SDL_ConvertEventToRenderCoordinates(renderer, event);
        swap(evil, evil_o);
        evil.x = event->motion.x;
        evil.y = event->motion.y;
        // cout << evil.x << " " << evil.y << endl;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_R) evil_a ^= 1;
    }

    if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        if(event->wheel.y > 0) force += 1;
        else force -= 1;
    }

    return SDL_APP_CONTINUE;
}

// https://stackoverflow.com/a/48291620
// void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
// {
//    const int32_t diameter = (radius * 2);

//    int32_t x = (radius - 1);
//    int32_t y = 0;
//    int32_t tx = 1;
//    int32_t ty = 1;
//    int32_t error = (tx - diameter);

//    while (x >= y)
//    {
//       //  Each of the following renders an octant of the circle
//       SDL_RenderPoint(renderer, centreX + x, centreY - y);
//       SDL_RenderPoint(renderer, centreX + x, centreY + y);
//       SDL_RenderPoint(renderer, centreX - x, centreY - y);
//       SDL_RenderPoint(renderer, centreX - x, centreY + y);
//       SDL_RenderPoint(renderer, centreX + y, centreY - x);
//       SDL_RenderPoint(renderer, centreX + y, centreY + x);
//       SDL_RenderPoint(renderer, centreX - y, centreY - x);
//       SDL_RenderPoint(renderer, centreX - y, centreY + x);

//       if (error <= 0)
//       {
//          ++y;
//          error += ty;
//          ty += 2;
//       }

//       if (error > 0)
//       {
//          --x;
//          tx += 2;
//          error += (tx - diameter);
//       }
//    }
// }

bool inside(int x, int y) {
    return (x >= 0 and x <= WINDOW_WIDTH) and (y >= 0 and y <= WINDOW_HEIGHT);
}

// in evil :pray: :sob:
bool inevil(SDL_FPoint &pnt) {
    float dx = pnt.x - evil.x;
    float dy = pnt.y - evil.y;
    return dx*dx + dy*dy < ev_box*ev_box;
}

// fixing collisions.
void col_fix() {
    for(int i = 0; i < n_pnts; i++) {
        int x = (int)points[i].x, y = (int)points[i].y;
        for(int j = 0 ; j < c_box ; j++) {
            for(int k = -c_box/2 ; k <= c_box/2 ; k++) {
                // if(j==0 and k==0) continue;
                int nx = x + j, ny = y + k;
                if(inside(nx, ny) and where[nx][ny] != -1) {
                    int v = where[nx][ny];
                    fix_col(points[i], points[v], velocity[i], velocity[v]);
                }
            }
        }
    }

    // evil point :3
    if(evil_a) {
        // SDL_FPoint evil_vel = {evil.x - evil_o.x, evil.y - evil_o.y};
        SDL_FPoint evil_vel = {force, force};
        // evil_vel.x *= 10;
        // evil_vel.y *= 10;
        SDL_FPoint oldevil = evil;
        SDL_FRect evil_rect = {evil.x-ev_box, evil.y-ev_box, 2*ev_box, 2*ev_box};
        for(int i = 0 ; i < n_pnts ; i++) {
            // attract(evil, points[i], evil_vel, velocity[i]);
            attract(points[i], evil, velocity[i], evil_vel);

            // sdl has every function bro
            if(inevil(points[i])) {
                // int u = 2*(velocity[i].x > 0) + (velocity[i].y > 0);
                // if(u&1) velocity[i].y *= -resti;
                // else velocity[i].x *= -resti;

                // if(u&2) velocity[i].x *= -resti;
                // else velocity[i].y *= -resti;
                
                // // // binary search while on ep s.t. after ep sec its still inside :3
                // float ep = 2047.875f;
                // for(float j = 1024.f ; j >= 0.01 ; j /= 2) {
                //     SDL_FPoint np = points[i];
                //     ep -= j;
                //     np.x = ep * velocity[i].x;
                //     np.y = ep * velocity[i].y;
                //     if(inevil(np)) ep += j;
                // }

                // points[i].x += ep * velocity[i].x;
                // points[i].y += ep * velocity[i].y;
                // if(SDL_PointInRectFloat(&points[i], &evil_rect)) {*/
                // }

                // // drawing in paper helps guys
                // float pl = points[i].x - l;
                // float pr = t - points[i].x;
                // float pt = points[i].y - t;
            }
        }

        evil = oldevil;
    }
}

// circle, copied from the internet :P
void draw_circle(SDL_Renderer* renderer, int rx, int ry, int r) {
    for(int x = 0; x <= r; ++x) {
        int y = (int)round(sqrt(r * r - x * x));

        SDL_RenderPoint(renderer, rx + x, ry + y);
        SDL_RenderPoint(renderer, rx + x, ry - y);
        SDL_RenderPoint(renderer, rx - x, ry + y);
        SDL_RenderPoint(renderer, rx - x, ry - y);

        SDL_RenderPoint(renderer, rx + y, ry + x);
        SDL_RenderPoint(renderer, rx + y, ry - x);
        SDL_RenderPoint(renderer, rx - y, ry + x);
        SDL_RenderPoint(renderer, rx - y, ry - x);
    }
}

long long last_c = SDL_GetPerformanceCounter();
/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    // cout << SDL_GetPerformanceCounter() << endl;
	// |F| = \sum 1/r^2
    // for(int i = 0 ; i < n_pnts ; i++) quad.insert(points[i]);

    Qdtree quad;
    quad.build();
    for(int i = 0 ; i < n_pnts ; i++) {
        SDL_FPoint w = quad.vel_calc(quad.root, points[i]);
        velocity[i].x += alpha * w.x;
        velocity[i].y += alpha * w.y;
        velocity[i].x /= 10.f;
        velocity[i].y /= 10.f; // to make stuff slow
    }

    for(int i = 0 ; i < n_pnts ; i++) {
        int ix = (int)points[i].x, iy = (int)points[i].y;
        if(!inside(ix, iy)) {
            if(ix > WINDOW_WIDTH) ix = WINDOW_WIDTH;
            if(ix < 0) ix = 0;
            if(iy > WINDOW_HEIGHT) iy = WINDOW_HEIGHT;
            if(iy < 0) iy = 0;
        }

        where[ix][iy] = -1;
    	points[i].x += velocity[i].x * alpha;
    	points[i].y += velocity[i].y * alpha;

        wall(points[i], velocity[i]);
        int nix = (int)points[i].x, niy = (int)points[i].y;
        if(!inside(nix, niy)) {
            if(nix > WINDOW_WIDTH) nix = WINDOW_WIDTH;
            if(nix < 0) nix = 0;
            if(niy > WINDOW_HEIGHT) niy = WINDOW_HEIGHT;
            if(niy < 0) niy = 0;
        }

        where[nix][niy] = i;
    }

    col_fix();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    // SDL_RenderPoints(renderer, points, n_pnts);  /* draw all the points! */

    for(int i = 0 ; i < n_pnts ; i++) {
        SDL_SetRenderDrawColor(renderer, clrs[i][0], clrs[i][1], clrs[i][2], SDL_ALPHA_OPAQUE);
        SDL_FRect nrect = {points[i].x-c_box/2, points[i].y-c_box/2, c_box/2, c_box/2};
        SDL_RenderFillRect(renderer, &nrect);
    }

    if(evil_a) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
        SDL_FRect evil_rect = {evil.x-ev_box, evil.y-ev_box, 2*ev_box, 2*ev_box};
        draw_circle(renderer, evil.x, evil.y, ev_box);
    }

    // draw rectangles of quadtree (debugging only :3)
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 64);
    // vector<Rect> rects = quad.DrawQd(quad.root, 0);
    // for(Rect rec: rects) {
    //     SDL_FRect nrec;
    //     nrec.x = (float)rec.x;
    //     nrec.y = (float)rec.y;
    //     nrec.h = (float)(rec.u - rec.x);
    //     nrec.w = (float)(rec.v - rec.y);
    //     SDL_RenderRect(renderer, &nrec);
    // }

    // outputting FPS:
    // cout << SDL_GetPerformanceCounter() << endl;
    frame_n++;
    long long now = SDL_GetPerformanceCounter();
    double elapsed = (double)(now - last_c) / (double)SDL_GetPerformanceFrequency();

    if (elapsed >= 0.5) { // update twice a second
        fps = (float)(frame_n / elapsed);
        frame_n = 0;
        last_c = now;
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDebugTextFormat(renderer, WINDOW_WIDTH - 120, 10, "FPS: %.1f", fps);
    SDL_RenderDebugTextFormat(renderer, 10, WINDOW_HEIGHT - 10, "Press 'R' to toggle black hole");
    SDL_RenderDebugTextFormat(renderer, WINDOW_WIDTH - 350, WINDOW_HEIGHT - 10, "Scroll mouse whell to change its force: %dN", force);

    SDL_RenderPresent(renderer);
    quad.destroy(quad.root);
    quad.root = new Node;
    quad.root->rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    for(int i=0;i<n_pnts;i++){velocity[i].x *= 10.f;velocity[i].y *= 10.f;}
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { /* SDL will clean up the window/renderer for us. */ }