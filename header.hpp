// header.cpp: the header file
#include <iostream>
#include <cmath>
#include <vector>
#include <array>

#include <SDL3/SDL_rect.h>
using namespace std;
using dbl = long double;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
const int n_pnts = 7000;

// quadtree search para :3
const float theta = 1.f;
const float epsilon = 1.f;
const float alpha = 0.1f; // step

// funcs
// struct Point {
// 	float x, y;
// };

SDL_FPoint compute_acc(SDL_FPoint a, SDL_FPoint b, int massb);
float dis2(SDL_FPoint &a, SDL_FPoint &b);

// quadtree
struct Rect {
	float x, y;
	float u, v;
};

struct Node {
	Node *kids[4] = {nullptr, nullptr, nullptr, nullptr};
	SDL_FPoint avg = {0.f, 0.f}; // average.
	vector<SDL_FPoint> pnts; // only in leafs :3
	Rect rect; // the rectangle it covers.
	int mass = 0; // mass, since m = 1.f its just number of SDL_FPoints in a subtree.
};

struct Qdtree {
	Node* root;

    Qdtree();
    void build();
    int whichquad(Rect rect, SDL_FPoint &pnt);
    void insert(Node *u, SDL_FPoint &pnt, int depth);
    void propagate(Node *u, int depth);
    void destroy(Node* u); // freeing memory

    // void insert(Node* u, SDL_FPoint pnt, int depth);
    SDL_FPoint vel_calc(Node *u, SDL_FPoint &pnt);
};