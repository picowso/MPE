all:
	g++ -O3 main.cpp physics.cpp quadtree.cpp header.hpp -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3 -o mpe