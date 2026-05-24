source emsdk/emsdk_env.fish

em++ -std=c++20 -O3 \
    -sALLOW_MEMORY_GROWTH=1 \
    -o index.html main.cpp quadtree.cpp physics.cpp \
    $(pkg-config --cflags --libs sdl3) \
    -sUSE_SDL=3 \
    --shell-file shell.html
rm ../MPE-emscripten/*
mv index.html index.js index.wasm ../MPE-emscripten/
