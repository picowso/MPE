all:
	g++ main.cpp physics.cpp header.hpp -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3 -o mpe