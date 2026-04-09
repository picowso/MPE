# MPE: Msemen Physics Engine!

[Try it here!](https://brilliant-pasca-2af93f.netlify.app/)

MPE is a C++ Quadtree physics engine, uses SDL3 for visualation, but the main production goal is the web using [Emscripten](https://emscripten.org/).

it's able to run 7000 points at an average of 15.3fps in my old thinkpad so it's nice, there is MASSIVE rooms for improvement when it comes to the performence, that's where my main focus is!

### Running natively

just run `make`, make sure you have `make` and also you compiled/installed `SDL3`, to compile for web run `./deploy_web.fish` using any shell actually!

### Physics

I used physics concepts like elasticity, newton's law of universal gravitation which is mainly what this 'engine' is based on.


I was forced to learn about integrators, but i settled with the simplest one, which is "euler integrator", basically just adding the change each time, basically riemann sum.