source emsdk/emsdk_env.fish

em++ -O2 -std=c++17 \
    main.cpp physics.cpp quadtree.cpp \
    -sUSE_SDL=3 \
    # -sMINIMAL_RUNTIME=0 \
    --shell-file shell.html \
    -o index.html

mv index.html index.js index.wasm ../MPE-emscripten/
