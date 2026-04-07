// physics.cpp: where the maths happen
#include "header.hpp"

SDL_FPoint compute_acc(SDL_FPoint a, SDL_FPoint b, int massb) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
    float dis = dx*dx + dy*dy + epsilon;
    float inv = 1./sqrtf(dis);
    float inv3 = massb * inv * inv * inv;
    return {dx * inv3, dy * inv3};
}

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