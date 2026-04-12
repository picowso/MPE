// quadtree.cpp: Quadtree! https://en.wikipedia.org/wiki/Quadtree
#include "header.hpp"

const int LIM = 11;
extern SDL_FPoint points[n_pnts], velocity[n_pnts];

Qdtree::Qdtree() {
	root = new Node;
	root->rect.x = 0;
	root->rect.y = 0;
	root->rect.u = WINDOW_WIDTH;
	root->rect.v = WINDOW_HEIGHT;
}

// void alloc(Node*& u) {
// 	// for(int i = 0 ; i < 4 ; i++) {
// 	// 	if(u->kids[i] == nullptr) u->kids[i] = new Node;
// 	// }

// 	if(u == nullptr) u = new Node;
// }

int Qdtree::whichquad(Rect rect, SDL_FPoint &pnt) {
	float midx = (rect.x + rect.u) / 2;
	float midy = (rect.y + rect.v) / 2;
	return 2*(pnt.x > midx) + (pnt.y > midy);
}

void Qdtree::insert(Node *u, SDL_FPoint &pnt, int depth) {
	if(depth == LIM) {
		u->pnts.push_back(pnt);
		u->mass++;
		return;
	}

	// alloc(u);
	int idx = whichquad(u->rect, pnt);
	Rect nxt = u->rect;
	float midx = (u->rect.x + u->rect.u) / 2;
	float midy = (u->rect.y + u->rect.v) / 2;

	if(idx & 1) nxt.y = midy;
	else nxt.v = midy;
	if(idx & 2) nxt.x = midx;
	else nxt.u = midx;
	// alloc(u->kids[idx]);
	if(u->kids[idx] == nullptr) u->kids[idx] = new Node;
	u->kids[idx]->rect = nxt;

	insert(u->kids[idx], pnt, depth+1);
}

void Qdtree::propagate(Node *u, int depth) {
	if(u == nullptr) return;

	// if(u->mass == 0) u->has_empty = 1;
	if(depth == LIM) {
		if(u->mass == 0) return;
		int un = u->pnts.size();
		// int r = rand()%un;
		for(SDL_FPoint k: u->pnts) {
			u->avg.x += k.x;
			u->avg.y += k.y;
		}

		// for(int i = 0 ; i < un ; i++) {
		// 	for(int r = 0 ; r < un ; r++) {
		// 		if(i == r) continue;
		// 		// fix_col();
		// 		fix_col(points[i], points[r], velocity[i], velocity[r]);
		// 	}
		// }

		u->avg.x /= (float)u->mass;
		u->avg.y /= (float)u->mass;
		return;
	}

	for(int i = 0 ; i < 4 ; i++) {
		propagate(u->kids[i], depth+1);
		if(u->kids[i] != nullptr) {
			u->mass += u->kids[i]->mass;
			// u->has_empty |= u->kids[i]->has_empty;
		}
	}

	if(u->mass == 0) return;

	for(int i = 0 ; i < 4 ; i++) {
		if(u->kids[i] != nullptr) {
			u->avg.x += u->kids[i]->avg.x * u->kids[i]->mass;
			u->avg.y += u->kids[i]->avg.y * u->kids[i]->mass;
		}
	}

	u->avg.x /= (float)u->mass;
	u->avg.y /= (float)u->mass;
}

void Qdtree::build() {
	for(int i = 0 ; i < n_pnts ; i++) insert(root, points[i], 0);
	propagate(root, 0);
	// collision_move(root, 0);
}

// figure this out later
// void collision_move(Node *u) { }

// vel calc
SDL_FPoint Qdtree::vel_calc(Node *u, SDL_FPoint &pnt) {
	float s = u->rect.u - u->rect.x;
	float d = dis2(pnt, u->avg);
	SDL_FPoint acc = {0.f, 0.f};

	if(d == 0) d = epsilon;
	if(s*s < theta*theta*d) {
		SDL_FPoint w = compute_acc(pnt, u->avg, u->mass);
		acc.x += w.x;
		acc.y += w.y;
		return acc;
	}

	for(int i = 0 ; i < 4 ; i++) {
		if(u->kids[i] != nullptr) {
			SDL_FPoint w = vel_calc(u->kids[i], pnt);
			acc.x += w.x;
			acc.y += w.y;
		}
	}

	return acc;
}

void Qdtree::destroy(Node* u) {
    if(!u) return;
    for (int i = 0; i < 4; i++) destroy(u->kids[i]);
    // u->mass = 0;
	// u->rect = {0,0,0,0};
	// u->avg = {0.f, 0.f};
    delete u;
}

vector<Rect> Qdtree::DrawQd(Node *u, int depth) {
	if(depth == 7) return {};
	vector<Rect> res;
	res.push_back(u->rect);
	for(int i = 0 ; i < 4 ; i++) {
		if(u->kids[i] != nullptr) {
			auto w = DrawQd(u->kids[i], depth + 1);
			for(Rect k: w) res.push_back(k);
		}
	}

	return res;
}