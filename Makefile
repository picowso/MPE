all:
	g++ -O3 main.cpp physics.cpp quadtree.cpp header.hpp -I/usr/local/include -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL3 -o mpe
web:
	source emsdk/emsdk_env.fish
	
	em++ -O2 -std=c++17 \
	  main.cpp physics.cpp quadtree.cpp \
	  -sUSE_SDL=3 \
	  -o index.html

	mv index.html index.js index.wasm ../MPE-emscripten/