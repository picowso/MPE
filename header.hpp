// header.cpp: the header file
#include <iostream>
#include <cmath>
#include <vector>
#include <array>

#include <SDL3/SDL_rect.h>
using namespace std;
using dbl = long double;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
const int n_pnts = 4000;

const float theta = 1.f; // quadtree search para
const float epsilon = 0.5f;
const float alpha = 0.39f; // step
const int rec_s = 4;
const int c_box = 7; // collision box size :3
const int ev_box = 25; // evil box size
const float resti = 1.f; // elasticity

// funcs
// struct Point {
// 	float x, y;
// };

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
	int frame = 0;
};

struct Qdtree {
	Node* root;
    vector<Node> pool; // let's see if this works :3

    Qdtree();
    void build();
    int whichquad(Rect rect, SDL_FPoint &pnt);
    void insert(Node *u, SDL_FPoint &pnt, int depth);
    void propagate(Node *u, int depth);
    void destroy(Node* u); // freeing memory

    // void insert(Node* u, SDL_FPoint pnt, int depth);
    SDL_FPoint vel_calc(Node *u, SDL_FPoint &pnt);

    // debugging func
    vector<Rect> DrawQd(Node *u, int depth);
};

// physics:
void wall(SDL_FPoint& p, SDL_FPoint& v);
void fix_col(SDL_FPoint& pntf, SDL_FPoint& pntt, SDL_FPoint& velf, SDL_FPoint& velt);
void attract(SDL_FPoint& pntf, SDL_FPoint& pntt, SDL_FPoint& velf, SDL_FPoint& velt);
SDL_FPoint compute_acc(SDL_FPoint a, SDL_FPoint b, int massb);
float dis2(SDL_FPoint &a, SDL_FPoint &b);