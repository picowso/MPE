// physics.cpp: where the maths happen
#include "header.hpp"

const float resti = 0.9f; // elasticity
// this was kinda helpful:
// https://ocw.mit.edu/courses/3-91-mechanical-behavior-of-plastics-spring-2007/f6536249600e453536e04776dbb876a1_01_elast.pdf

float dis2(SDL_FPoint &a, SDL_FPoint &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return dx*dx + dy*dy;
}

float angle(SDL_FPoint &a, SDL_FPoint &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return atan2(dy, dx);
}

// reverse vel when hit with window edges
void wall(SDL_FPoint& p, SDL_FPoint& v) {
    if (p.x < 1) {
        p.x = 1;
        v.x = -v.x * resti;
    }

    if (p.x >= WINDOW_WIDTH) {
        p.x = WINDOW_WIDTH;
        v.x = -v.x * resti;
    }

    if (p.y < 1) {
        p.y = 1;
        v.y = -v.y * resti;
    }

    if (p.y >= WINDOW_HEIGHT) {
        p.y = WINDOW_HEIGHT;
        v.y = -v.y * resti;
    }
}

// computes acceleration
SDL_FPoint compute_acc(SDL_FPoint a, SDL_FPoint b, int massb) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
    float dis = dx*dx + dy*dy + epsilon;
    float inv = 1./sqrtf(dis);
    float inv3 = massb * inv * inv * inv;
    return {dx * inv3, dy * inv3};
}

// already assumes collision, just fixes it
void fix_col(SDL_FPoint& pntf, SDL_FPoint& pntt, SDL_FPoint& velf, SDL_FPoint& velt) {
	// direction
	float dx = pntf.x - pntt.x;
	float dy = pntf.y - pntt.y;
	float dis = sqrtf(dx*dx + dy*dy);

	if(dis == 0.f) {
		// cout << "AWILI" << endl;
		return;
	}

	// collision normal
	float nx = dx / dis;
	float ny = dy / dis;

	float dvx = velf.x - velt.x;
	float dvy = velf.y - velt.y;

	float spd = dvx*nx + dvy*ny;
	// already moving other dir 
	if(spd > 0.f) return;

	// so they aren't comfortable with low velocity yet near each other :P
	spd = min(spd, -0.001f);

	float ret = spd*(1+resti)/2.f;
	velf.x -= ret*nx;
	velf.y -= ret*ny;
	velt.x += ret*nx;
	velt.y += ret*ny;
}