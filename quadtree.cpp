#include "header.hpp"

const int LIM = 31;

struct Qdtree {
	Qdtree *lr = nullptr;
	Qdtree *ll = nullptr;
	Qdtree *rr = nullptr;
	Qdtree *rl = nullptr;
	SDL_FPoint avg = {0.f, 0.f}; // average
	int cnt = 0;
};

Qdtree* insert(SDL_FPoint pnt) {

}

Qdtree* remove(SDL_FPoint pnt) {

}

